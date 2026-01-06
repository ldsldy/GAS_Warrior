// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/WarriorCharacterAnimInstance.h"
#include "WarriorHeroAnimInstance.generated.h"

/**
플레이어 캐릭터만을 위한 애니메이션 인스턴스 클래스
 */

class AWarriorHeroCharacter;

UCLASS()
class WARRIOR_API UWarriorHeroAnimInstance : public UWarriorCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Refrences")
	TWeakObjectPtr<AWarriorHeroCharacter> OwningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	// 플레이어가 움직이지 않고 있는 상태가 이 임계값을 초과하면 릴렉스 상태로 진입
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	// 플레이어가 움직이지 않은 시간
	float IdleElapsedTime;
};
