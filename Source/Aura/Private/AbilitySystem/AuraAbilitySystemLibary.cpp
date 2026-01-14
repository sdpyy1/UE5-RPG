// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibary.h"
#include <Kismet/GameplayStatics.h>
#include <UI/HUD/AuraHUD.h>
#include <Player/AuraPlayerState.h>
#include "UI/WidgetController/AuraWidgetController.h"

UAuraOverlayWidgetController* UAuraAbilitySystemLibary::GetOverlayWidgetController(const UObject* UWorldContextObject)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(UWorldContextObject, 0);
	check(PC);
	if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD())) {
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
		return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

UAuraAttributeMenuController* UAuraAbilitySystemLibary::GetAttributeMenuController(const UObject* UWorldContextObject)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(UWorldContextObject, 0);
	check(PC);
	if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD())) {
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
		return AuraHUD->GetAttributeMenuController(WidgetControllerParams);
	}
	return nullptr;
}