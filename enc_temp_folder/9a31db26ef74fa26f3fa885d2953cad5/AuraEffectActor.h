// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraItemActor.h"
#include "AuraEffectActor.generated.h"
class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AAuraItemActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	/*
		给一个Actor施加一个GameplayEffect
	*/
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;
};
