// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"

#include "AuraAbilitySystemLibary.generated.h"
class UAuraOverlayWidgetController;
class UAuraAttributeMenuController;
class UAbilitySystemComponent;
/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibary|WidgetController")
	static UAuraOverlayWidgetController* GetOverlayWidgetController(const UObject* UWorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibary|WidgetController")
	static UAuraAttributeMenuController* GetAttributeMenuController(const UObject* UWorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibary|CharacterClassDefaults")
	static void InitDefaultAttributes(const UObject* UWorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);
};
