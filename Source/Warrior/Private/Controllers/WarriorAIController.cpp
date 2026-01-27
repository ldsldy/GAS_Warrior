// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "WarriorDebugHealper.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true; // 적 감지 활성화
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false; // 아군 감지 비활성화
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false; // 중립 감지 비활성화
	AISenseConfig_Sight->SightRadius = 5000.f; // 시야 반경 설정
	AISenseConfig_Sight->LoseSightRadius = 0.f; // 시야 상실 반경 설정
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; // 주변 시야 각도 설정
	
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight); // 시야 감각 구성
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass()); // 지배적인 감각 설정
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other); // Other가 APawn인지 확인

	const IGenericTeamAgentInterface* OtherTeamAgent = 
		Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	// 다른 팀 에이전트의 ID가 이 컨트롤러의 팀 ID와 다르면,
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile; // 적대적인 태도로 간주
	}

	return ETeamAttitude::Friendly; // 그렇지 않으면 우호적인 태도로 간주
}
void AWarriorAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (DetourCrowdAvoidaneQuality)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);	 break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);	 break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);	 break;
		default:
			break;
		}

		CrowdComp->SetAvoidanceGroup(1);	// AI 캐릭터가 속한 회피 그룹 설정
		CrowdComp->SetGroupsToAvoid(1);		// AI 캐릭터가 회피할 그룹 설정
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}
void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		// 블랙보드에 이미 TargetActor가 설정되어 있지 않은 경우
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			// 감지가 성공적이고 Actor가 유효한 경우
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
}
