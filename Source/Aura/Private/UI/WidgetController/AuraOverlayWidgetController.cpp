// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include <AbilitySystem/AuraAbilitySystemComponent.h>
#include <Aura/Aura.h>

void UAuraOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* attributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Broadcast Delegate
	OnHealthChanged.Broadcast(attributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(attributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(attributeSet->GetMana());
	OnMaxManaChanged.Broadcast(attributeSet->GetMaxMana());
}

void UAuraOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* attributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(attributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(attributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnMaxHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(attributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnManaChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(attributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnMaxManaChanged.Broadcast(Data.NewValue); });

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda([this](const FGameplayTagContainer& TagContainer) {
		for (const FGameplayTag& Tag : TagContainer) {
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

			if (Tag.MatchesTag(MessageTag)) {
				const FUIWidgetRow* Row = GetDataRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				check(Row);
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
		}
		});
}