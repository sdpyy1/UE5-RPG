// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <AbilitySystem/AuraAbilitySystemComponent.h>

#include <Player/AuraPlayerState.h>
#include <Player/AuraPlayerController.h>
#include <UI/HUD/AuraHUD.h>
AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	// 用movementComponent来更好的处理转向
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	/////////////////////////////  GAS /////////////////////////////
	// Aura的GAS放在了AuraPlayerState中
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* playerState = GetPlayerState<AAuraPlayerState>();
	check(playerState);
	return playerState->GetPlayerLevel();
}

UAbilitySystemComponent* AAuraCharacter::GetAbilitySystemComponent() const
{
	AAuraPlayerState* playerState = GetPlayerState<AAuraPlayerState>();
	return playerState->GetAbilitySystemComponent();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* playerState = GetPlayerState<AAuraPlayerState>();

	if (!playerState) {
		return;
	}
	check(playerState);
	// ASC的owner是playerState，但是avatar是character
	playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
	Cast<UAuraAbilitySystemComponent>(playerState->GetAbilitySystemComponent())->AbilityCharacterInfoSet();

	AbilitySystemComponent = playerState->GetAbilitySystemComponent();

	AttributeSet = playerState->GetAttributeSet();

	if (AAuraPlayerController* auraPlayerController = Cast<AAuraPlayerController>(GetController())) {
		if (AAuraHUD* auraHUD = Cast<AAuraHUD>(auraPlayerController->GetHUD())) {
			auraHUD->InitOverlay(auraPlayerController, playerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}