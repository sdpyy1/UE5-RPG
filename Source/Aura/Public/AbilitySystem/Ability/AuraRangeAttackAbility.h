// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraRangeAttackAbility.generated.h"
class AAuraProjectileActor;
/**
 *	远程攻击能力
 */
class AActor;
UCLASS()
class AURA_API UAuraRangeAttackAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
protected:
	
	/*
		在指定位置生成投掷物
	*/
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(AActor* Attacker, const FVector& TargetLocation);


	/*
		投掷物类型
	*/
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectileActor> ProjectileActorClass;
};
