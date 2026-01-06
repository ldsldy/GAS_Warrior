// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorBaseCharacter.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

AWarriorBaseCharacter::AWarriorBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// 데칼이 캐릭터에 영향을 미치지 않도록 설정
	GetMesh()->SetReceivesDecals(false);

	WarriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	WarriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));
}

UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
	return GetWarriorAbilitySystemComponent();
}

void AWarriorBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (WarriorAbilitySystemComponent)
	{
		// 소유자 액터는 논리적 소유자, 아바타 액터는 월드에 존재하는 액터
		WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}


