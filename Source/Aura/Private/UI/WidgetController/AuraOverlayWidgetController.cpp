// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include <AbilitySystem/AuraAttributeSet.h>

void UAuraOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* attributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Broadcast Delegate
	OnHealthChanged.Broadcast(attributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(attributeSet->GetMaxHealth());
}

void UAuraOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* attributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(attributeSet->GetHealthAttribute()).AddUObject(this, &UAuraOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(attributeSet->GetMaxHealthAttribute()).AddUObject(this, &UAuraOverlayWidgetController::MaxHealthChanged);
}

void UAuraOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}