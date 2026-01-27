// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/WarriorBaseAnimInstance.h"
#include "WarriorCharacterAnimInstance.generated.h"

class AWarriorBaseCharacter;
class UCharacterMovementComponent;

/**
적과 플레이어 캐릭터가 공유하는 애니메이션 인스턴스 클래스
 */
UCLASS()
class WARRIOR_API UWarriorCharacterAnimInstance : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	// 쓰레드를 활용하여 안전하고 병렬적으로 업데이트하는 함수
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	// 애님 인스턴스는 단순 참조를 사용하여 소유 캐릭터에 접근, 순환 참조 방지
	UPROPERTY()
	TWeakObjectPtr<AWarriorBaseCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwningCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};
