// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHealper.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

// FGameplayEffectModCallbackData : EffectSpec, EvaluatedData(수정 중인 어트리뷰트), Target 등
void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// CurrentHealth 어트리뷰트가 수정된 경우
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
	}

	// CurrentRage 어트리뷰트가 수정된 경우
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);
	}

	// DamageTaken 어트리뷰트가 수정된 경우
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		// 현재 체력에서 받은 데미지 만큼 차감한 후 적용
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		
		SetCurrentHealth(NewCurrentHealth);
		
		const FString DebugString = FString::Printf(
			TEXT("Old Health : %f, Damage Done : %f, NewCurrentHealth : %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth
		);

		Debug::Print(DebugString, FColor::Green);

	    //TODO : Notify the UI

		// 사망 처리(이 어트리뷰트 셋이 적용된 액터에 사망 태그 추가)
		if (NewCurrentHealth == 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Death);
			
		}
	}
}
