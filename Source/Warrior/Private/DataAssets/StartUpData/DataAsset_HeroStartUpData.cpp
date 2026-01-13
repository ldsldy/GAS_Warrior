// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

void UDataAsset_HeroStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	// 부모의 기본 어빌리티 부여 로직 실행
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	// 플레이어만의 히어로 어빌리티 세트 부여
	for (const FWarriorHeroAbilitySet& AbilitySet : HeroStartUpAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant); // 이 어빌리티 스펙 생성
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();    // 이 어빌리티를 부여한 소스 오브젝트 설정
		AbilitySpec.Level = ApplyLevel;                              // 어빌리티 레벨 설정
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);  // 입력 태그 추가(나중에 이 태그로 어빌리티 검색 가능)
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
