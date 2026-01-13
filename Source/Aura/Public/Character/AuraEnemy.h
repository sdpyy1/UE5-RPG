// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"

#include "AuraEnemy.generated.h"
/**
 *
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	/*
		HigtLight Interface
	*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	/*
		Combat Interface
	*/
	virtual int GetPlayerLevel() override;

protected:
	virtual void BeginPlay() override;
protected:
	virtual void InitAbilityActorInfo() override;
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default", meta = (AllowPrivateAccess = true))
	int32 Level = 1;
};
