// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include <UI/HUD/AuraHUD.h>
#include <Player/AuraPlayerState.h>
#include "UI/WidgetController/AuraWidgetController.h"
#include <Game/AuraGameModeBase.h>
#include <AbilitySystemComponent.h>

#include "Interaction/CombatInterface.h"

UAuraOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* UWorldContextObject)
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

UAuraAttributeMenuController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* UWorldContextObject)
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

void UAuraAbilitySystemLibrary::InitDefaultAttributes(const UObject* UWorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
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

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereLocation,
	float Radius)
{
	FCollisionQueryParams SphereCollision;
	SphereCollision.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereCollision);
	for (FOverlapResult Overlap : Overlaps)
	{
		if (Overlap.GetActor())
		{
			if (const bool Implements_Combat = Overlap.GetActor()->Implements<UCombatInterface>())
			{

				if (Implements_Combat)
				{
					const bool isAlive = !ICombatInterface::Execute_isDead(Overlap.GetActor());
					if (isAlive)
					{
						OutOverlappingActors.AddUnique(Overlap.GetActor());
					}
				}
				
			}
			

			
		}
	}
}

FTaggedMontage UAuraAbilitySystemLibrary::GetRandomTaggedMontage(TArray<FTaggedMontage> AttackMontages)
{
	if (int num = AttackMontages.Num(); num > 0)
	{
		const int32 Index = FMath::RandRange(0, num - 1);
		return AttackMontages[Index];
	}
	return {};
}

bool UAuraAbilitySystemLibrary::isEnemy(AActor* First, AActor* Second)
{
	return First->ActorHasTag("Enemy")? Second->ActorHasTag("Player") : Second->ActorHasTag("Enemy");
}
