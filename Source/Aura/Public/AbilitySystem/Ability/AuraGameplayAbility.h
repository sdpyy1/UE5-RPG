// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "AuraGameplayAbility.generated.h"

/**
 *	GameAbility基类
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	/*
		激活能力的接口
	*/
	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;



	/*
		每种能力都需要手动设置启动标签，表示什么Input来激活能力
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupTag;
};
