// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibary.h"
#include <Kismet/GameplayStatics.h>
#include <UI/HUD/AuraHUD.h>
#include <Player/AuraPlayerState.h>
#include "UI/WidgetController/AuraWidgetController.h"
#include <Game/AuraGameModeBase.h>
#include <AbilitySystemComponent.h>

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

void UAuraAbilitySystemLibary::InitDefaultAttributes(const UObject* UWorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(UWorldContextObject));
	if (!AuraGameMode) return;

	AActor * AvatarActor = ASC->GetAvatarActor();

	FCharacterClassDefaultInfo DefaultInfo = AuraGameMode->CharacterClassInfo->CharacterClassInformation[CharacterClass];

	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
    PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(DefaultInfo.PrimaryAttributes, Level, PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());


    FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
    SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondSpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondSpecHandle.Data.Get());


    FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
    VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->VitalAttributes, Level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}