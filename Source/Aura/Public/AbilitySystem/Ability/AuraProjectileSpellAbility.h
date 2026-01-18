// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraProjectileSpellAbility.generated.h"
class AAuraProjectileActor;
/**
 *	投掷物的能力基类
 */
UCLASS()
class AURA_API UAuraProjectileSpellAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/*
		在指定位置生成投掷物
	*/
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& TargetLocaltion);


	/*
		投掷物类型
	*/
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectileActor> ProjectileActorClass;


	/*
		GameplayEffect类型，用于计算伤害
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
