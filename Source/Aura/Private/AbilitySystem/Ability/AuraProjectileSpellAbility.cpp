// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Ability/AuraProjectileSpellAbility.h"
#include <Interaction/CombatInterface.h>
#include <Actor/AuraProjectileActor.h>

void UAuraProjectileSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool isSever = HasAuthority(&ActivationInfo);
	if (!isSever) return;

	ICombatInterface* CombatAPI = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatAPI) {
		const FVector SocketLocation = CombatAPI->GetCombatSocketLoaction();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		AAuraProjectileActor* SpawnActor = GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		SpawnActor->FinishSpawning(SpawnTransform);
	}
}