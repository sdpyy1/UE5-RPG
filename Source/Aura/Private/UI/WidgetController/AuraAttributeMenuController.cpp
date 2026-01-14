// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/AuraAttributeMenuController.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"
void UAuraAttributeMenuController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributesMap) {
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAuraAttributeMenuController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributesMap) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda([this, Pair, AS](const FOnAttributeChangeData& Data) {
			FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
			Info.AttributeValue = Pair.Value().GetNumericValue(AS);
			AttributeInfoDelegate.Broadcast(Info);
		});
	}
}