// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include <Aura/Aura.h>
#include <AbilitySystem/Ability/AuraGameplayAbility.h>
void UAuraAbilitySystemComponent::AbilityCharacterInfoSet()
{
	// 绑定AbilitySystemComponent自带的委托
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		// 赋予Ability一个指定的默认Tag
		if (UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability)) {
			AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartupTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) return;
	for (auto& AbilitySpec : GetActivatableAbilities()) {
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag)) {
			AbilitySpecInputPressed(AbilitySpec); // 只是激活一个AbilitySpec中的Bool
			if (!AbilitySpec.IsActive()) {
				TryActivateAbility(AbilitySpec.Handle); // 如果按下的Ability没有激活就激活他
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagRelease(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) return;
	for (auto& AbilitySpec : GetActivatableAbilities()) {
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag)) {
			AbilitySpecInputReleased(AbilitySpec); // 只告知ReleaseBool，因为有些Ability并不是松开就失效
		}

	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer tagContainer;
	EffectSpec.GetAllAssetTags(tagContainer);
	EffectAssetTagsDelegate.Broadcast(tagContainer);
} 