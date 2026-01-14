// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include <Interaction/EnemyInterface.h>
#include <Input/AuraEnhancedInputComponent.h>

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // 开启该Actor的网络同步
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI inputModelData;
	inputModelData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputModelData.SetHideCursorDuringCapture(false);
	SetInputMode(inputModelData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraEnhancedInputComponent* EnhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPress, &ThisClass::AbilityInputTagRelease, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAixVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		ControlledPawn->AddMovementInput(ForwardDirection, InputAixVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAixVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	// Trace Cursor and HighLight Enemy!
	FHitResult cursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, cursorHit); // 需要把Enemy的碰撞预设的碰撞响应的Visibiliy通道设置为Block
	if (!cursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(cursorHit.GetActor());
	if (LastActor == nullptr) {
		if (ThisActor != nullptr) {
			// HighLight
			ThisActor->HighlightActor();
		}
	}
	else {
		if (ThisActor == nullptr) {
			LastActor->UnHighlightActor();
		}
		else {
			if (ThisActor != LastActor) {
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPress(FGameplayTag GameplayTag)
{
	GEngine->AddOnScreenDebugMessage(1, 10, FColor::Blue, GameplayTag.ToString());
}

void AAuraPlayerController::AbilityInputTagRelease(FGameplayTag GameplayTag)
{
	GEngine->AddOnScreenDebugMessage(2, 10, FColor::Cyan, GameplayTag.ToString());
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag GameplayTag)
{
	GEngine->AddOnScreenDebugMessage(3, 10, FColor::White, GameplayTag.ToString());
}