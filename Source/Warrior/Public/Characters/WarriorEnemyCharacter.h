// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorEnemyCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()
	
public:
	AWarriorEnemyCharacter();

	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }

protected:
	virtual void PossessedBy(AController* NewController) override;

private:
	void InitEnemyStartUpData();

protected:
	UPROPERTY(ViSIBLEaNYWHERE, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

};
