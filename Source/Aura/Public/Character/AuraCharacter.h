// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();

	/*
		Spawn Character
		↓
		Spawn PlayerState
		↓
		Controller Possess Character
		↓
		调用 PossessedBy()  ← 服务器
	*/
	virtual void PossessedBy(AController* NewController) override;

	/*
		Spawn Character
		↓
		Character 已存在
		↓
		PlayerState 通过 Replication 过来
		↓
		OnRep_PlayerState() ← 客户端
	*/
	virtual void OnRep_PlayerState() override;

	/*
		Combat Interface
	*/
	virtual int32 GetPlayerLevel() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	virtual void InitAbilityActorInfo() override;
};
