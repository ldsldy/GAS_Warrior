// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()
	
	UBTService_OrientToTargetActor();

	// 서비스가 에셋에서 초기화될 때 호출되는 함수 재정의
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	// 서비스를 설명하는 문자열을 반환하는 함수 재정의
	virtual FString GetStaticDescription() const override;

	// 서비스 노드가 틱될 때 호출되는 함수 재정의
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationInterpSpeed;
};
