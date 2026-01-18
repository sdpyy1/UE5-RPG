// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Ability/AuraProjectileSpellAbility.h"
#include <Interaction/CombatInterface.h>
#include <Actor/AuraProjectileActor.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"

void UAuraProjectileSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpellAbility::SpawnProjectile(const FVector& TargetLocaltion)
{
	const bool isSever = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isSever) return;

	ICombatInterface* CombatAPI = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatAPI) {
		// 生成这个投掷物Actor
		const FVector SocketLocation = CombatAPI->GetCombatSocketLoaction();
		FRotator SocketRotation = (TargetLocaltion - SocketLocation).Rotation();
		SocketRotation.Pitch = 0.f;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(SocketRotation.Quaternion());
		AAuraProjectileActor* SpawnActor = GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ASC->MakeEffectContext());
		SpawnActor->DamageEffectSpecHandle = SpecHandle;
		SpawnActor->FinishSpawning(SpawnTransform);
	}
}