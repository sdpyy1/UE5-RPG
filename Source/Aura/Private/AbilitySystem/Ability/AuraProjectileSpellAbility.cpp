// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Ability/AuraProjectileSpellAbility.h"
#include <Interaction/CombatInterface.h>
#include <Actor/AuraProjectileActor.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

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
		const FVector SocketLocation = CombatAPI->Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
		FRotator SocketRotation = (TargetLocaltion - SocketLocation).Rotation();
		SocketRotation.Pitch = 0.f;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(SocketRotation.Quaternion());
		AAuraProjectileActor* SpawnActor = GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ASC->MakeEffectContext());

		FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		const float DamageEval = Damage.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Damage, DamageEval);
		SpawnActor->DamageEffectSpecHandle = SpecHandle;
		SpawnActor->FinishSpawning(SpawnTransform);
	}
}