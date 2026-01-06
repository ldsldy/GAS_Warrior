// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WarriorBaseAnimInstance.generated.h"

class AWarriorBaseCharacter;
class UCharacterMovementComponent;
/**
 헬퍼 함수가 포함된 기본 애니메이션 인스턴스 클래스
 */
UCLASS()
class WARRIOR_API UWarriorBaseAnimInstance : public UAnimInstance
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
};
