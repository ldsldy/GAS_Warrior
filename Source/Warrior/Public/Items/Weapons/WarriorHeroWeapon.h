// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "GamePlayAbilitySpecHandle.h"
#include "WarriorHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWarriorHeroWeaponData HeroWeaponData;

	// 어빌리티 스펙 핸들을 저장하는 함수
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);

	// 어빌리티 스펙 핸들을 반환하는 함수
	UFUNCTION(BlueprintPure)
	TArray< FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
	// 어빌리티 삭제를 위한 어빌리티 스펙 핸들들
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
