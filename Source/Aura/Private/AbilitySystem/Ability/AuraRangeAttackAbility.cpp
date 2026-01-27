// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Ability/AuraRangeAttackAbility.h"
#include <Interaction/CombatInterface.h>
#include <Actor/AuraProjectileActor.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Components/SphereComponent.h"

void UAuraRangeAttackAbility::SpawnProjectile(AActor* Attacker, const FVector& TargetLocation)
{
	const bool isSever = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isSever) return;

	ICombatInterface* CombatAPI = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatAPI) {
		// 生成这个投掷物Actor
		const FVector SocketLocation = CombatAPI->Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),FAuraGameplayTags::Get().Montage_Attack_Weapon);
		FRotator SocketRotation = (TargetLocation - SocketLocation).Rotation();
		SocketRotation.Pitch = 0.f;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(SocketRotation.Quaternion());
		AAuraProjectileActor* SpawnActor = GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ASC->MakeEffectContext());

		FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		const float DamageEval = DamageComputer.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Damage, DamageEval);
		SpawnActor->DamageEffectSpecHandle = SpecHandle;
		SpawnActor->FinishSpawning(SpawnTransform);

		SpawnActor->Sphere->IgnoreActorWhenMoving(Attacker, true); // 忽略自己

	}
}
