// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"

#include "AuraAbilitySystemLibrary.generated.h"
class UAuraOverlayWidgetController;
class UAuraAttributeMenuController;
class UAbilitySystemComponent;
/**
 *	一些全局可用的静态函数
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibary|WidgetController")
	static UAuraOverlayWidgetController* GetOverlayWidgetController(const UObject* UWorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibary|WidgetController")
	static UAuraAttributeMenuController* GetAttributeMenuController(const UObject* UWorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibary|CharacterClassDefaults")
	static void InitDefaultAttributes(const UObject* UWorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	/*
	 *	查询一个球形范围内的活的角色
	 */
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibary|GameplayMechanics")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereLocation, float Radius);
};
