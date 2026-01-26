// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WarriorAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS() 
class WARRIOR_API AWarriorAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// AI 컨트로러의 기본 컴포넌트를 바꿈
	// 객체의 초기화 과정을 관리하는 FObjectInitializer를 매개변수로 커스터마이징
	AWarriorAIController(const FObjectInitializer& ObjectInitializer); 

	// 이 AI 컨트롤러가 다른 액터에 대해 어떤 태도를 가지는지 반환
	// ETeamAttitude::Type : AI가 상대를 어떻게 인식하는지 정의
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance = true;

	// bEnableDetourCrowdAvoidance가 true일 때만 편집 가능
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidaneQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;
};
