// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Widgets/WarriorWidgetBase.h"
#include "Controllers/WarriorHeroController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "WarriorDebugHealper.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	RestTargetLockMappingContext();
	ClearUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	// 잠금 대상이 없거나 죽은 상태면 능력 취소
	if (!CurrentLockedActor.IsValid() ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor.Get(), WarriorGameplayTags::Shared_Status_Death) ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Shared_Status_Death))
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockWidgetPosition();

	// 영웅의 방향을 잠금 대상 쪽으로 회전 시킬 경우와 아닐 경우 분리
	// 플레이어가 구르기 또는 방어 중일 때는 회전 무시
	const bool bShouldOverrideRotation =
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Rolling)
		&&
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking);

	if (bShouldOverrideRotation)
	{
		// 캐릭터가 돌아가야하는 방향 계산
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetHeroCharacterFromActorInfo()->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);

		// 컨트롤러가 돌아가야하는 방향 계산
		const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);
	
		// 실제 캐릭터와 컨트롤러 회전
		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	// 현재 잠금 대상 주변의 액터들 오른쪽, 왼쪽 분류
	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	// 잠금 가능한 대상이 없으면 능력 취소
	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	if (CurrentLockedActor.IsValid())
	{
		DrawTargetLcokWidget();

		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();
	TArray<FHitResult> BoxTraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true // 자기 자신은 무시
	);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	float CloseetDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(),
		InAvailableActors,
		CloseetDistance
	);
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor.IsValid() || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	// 현재 잠금 대상과 그 주변 액터들의 방향 벡터를 비교하여 왼쪽과 오른쪽으로 분류
	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor.Get()) continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);
	
		if (CrossResult.Z > 0.f)
		{
			// 오른손 법칙에 의해 Z 값이 양수면 오른쪽(적 기준)
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			// 음수면 왼쪽(적 기준)
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}

void UHeroGameplayAbility_TargetLock::DrawTargetLcokWidget()
{
	if(!DrawnTargetLockWidget.IsValid())
	{
		checkf(TargetLockWidgetClass, TEXT("TargetLockWidgetClass is nullptr in %s"), *GetName());

		DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

		check(DrawnTargetLockWidget.IsValid());

		DrawnTargetLockWidget->AddToViewport();
	}
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (!DrawnTargetLockWidget.IsValid() && !CurrentLockedActor.IsValid())
	{
		CancelTargetLockAbility();
		return;
	}

	// 위젯의 월드 위치를 화면 위치에 투영
	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true // 플레이어 뷰포트 스케일 고려
	);

	// 위젯의 크기를 가져오지 않았다면 가져오기
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		// WidgetTree란 UUserWidget 내의 위젯 계층 구조를 나타내는 객체입니다.
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}

	ScreenPosition -= (TargetLockWidgetSize / 2.f);

	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;

	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	// 로컬 플레이어 컨트롤러의 향상된 입력 서브시스템 가져오기
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem =  ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	// 매핑 컨텍스트 추가
	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	// 능력 취소
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::ClearUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;

	if (DrawnTargetLockWidget.IsValid())
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}

	DrawnTargetLockWidget = nullptr;
	TargetLockWidgetSize = FVector2D::ZeroVector;

	CachedDefaultMaxWalkSpeed = 0.f;
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	}
}

void UHeroGameplayAbility_TargetLock::RestTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo())
	{
		return;
	}
	// 로컬 플레이어 컨트롤러의 향상된 입력 서브시스템 가져오기
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	// 매핑 컨텍스트 제거
	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}
