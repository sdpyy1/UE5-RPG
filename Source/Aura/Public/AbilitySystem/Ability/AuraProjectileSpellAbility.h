// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraProjectileSpellAbility.generated.h"
class AAuraProjectileActor;
/**
 *	投掷物的能力基类
 */
UCLASS()
class AURA_API UAuraProjectileSpellAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
protected:
	
	/*
		在指定位置生成投掷物
	*/
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& TargetLocation);


	/*
		投掷物类型
	*/
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectileActor> ProjectileActorClass;
};
