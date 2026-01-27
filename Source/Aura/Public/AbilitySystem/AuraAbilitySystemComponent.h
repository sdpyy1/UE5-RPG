// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"
struct FGameplayTagContainer;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&)
/**
 *	AbilitySystemComponent
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	/*
		所有Character创建AbilitySystemComponent后都会调用这个
	*/
	void AbilityCharacterInfoSet();

	FEffectAssetTags EffectAssetTagsDelegate;

	/*
		给ASC添加一组Ability
	 */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	/*
		ASC负责查看这个Tag对应的Ability，并操作这个Ability
		用于需要按下案件才会激活的能力
	*/
	void AbilityInputTagHeld(const FGameplayTag& Tag);
	void AbilityInputTagRelease(const FGameplayTag& Tag);
protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
