// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHealper.h"

AWarriorWeaponBase::AWarriorWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 생성자에서 직렬화된 델리게이트 바인딩
	// 이후에 바인딩 제거 또는 변경이 불필요할 경우 여기서 바인딩, 그러나 런타임에 바인딩을 변경해야 할 경우 BeginPlay 등에서 바인딩
	// AddUniqueDynamic: 동일한 델리게이트에 여러 번 바인딩되는 것을 방지
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void AWarriorWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forgot to assign an Instigator for the weapon %s?"), *GetName());

	// 무기 소유자로 인스티게이터가 설정되었다면
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 자기 자신에게 충돌하지 않도록 함
		if (WeaponOwningPawn != HitPawn)
		{
			// PawnCombatComponent에게 알림
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}

		//TODO : Implement hit check for enemy characters
	}
}

void AWarriorWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forgot to assign an Instigator for the weapon %s?"), *GetName());

	// 무기 소유자로 인스티게이터가 설정되었다면
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			// PawnCombatComponent에게 알림
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}

		//TODO : Implement hit check for enemy characters
	}
}
