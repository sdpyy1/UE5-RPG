// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;

USTRUCT(Blueprintable)
struct FTaggedMontage
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	/* Tag用于指定接收在Montage中发送Notify携带的Tag， 在动画蒙太奇中还是得手动设置每个Notify是哪个标签*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MontageTag;

	/* 命中音效 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USoundBase * HitSound = nullptr;
};




// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};
class UAnimMontage;
/**
 *
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingVector(const FVector& Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
		
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem * GetBloodEffect();


	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool isDead() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor * GetAvatar();
	virtual void Die() = 0;

};
