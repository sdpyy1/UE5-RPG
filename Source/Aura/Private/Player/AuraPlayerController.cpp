// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include <Interaction/EnemyInterface.h>
#include <Input/AuraEnhancedInputComponent.h>
#include <AbilitySystem/AuraAbilitySystemComponent.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NAvigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextWidgetComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // 开启该Actor的网络同步

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
	AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlPawn = GetPawn()) {
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius) {
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraEnhancedInputComponent* EnhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	// Input转为对应的GameplayTag
	EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPress, &ThisClass::AbilityInputTagRelease, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* Target)
{
	if (IsValid(Target) && DamageTextClass) {
		UDamageTextWidgetComponent * DamgeTextComponentInstance = NewObject<UDamageTextWidgetComponent>(Target, DamageTextClass);
		DamgeTextComponentInstance->RegisterComponent();
		DamgeTextComponentInstance->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamgeTextComponentInstance->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        DamgeTextComponentInstance->SetDamagetText(Damage);
	}
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
	if (GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB)) {
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagRelease(FGameplayTag GameplayTag)
{
	if (!GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB)) {
		if (GetAuraASC()) {
			GetAuraASC()->AbilityInputTagRelease(GameplayTag);
		}
		return;
	}
	if (bTargeting) {
		if (GetAuraASC()) {
			GetAuraASC()->AbilityInputTagHeld(GameplayTag);
		}
	}
	else {
		if (APawn* ControlPawn = GetPawn()) {
			if (FollowTime <= ShortPressThreshold) {
				// 通过导航系统获得起点终点之间一系列点，然后把他们加入到SplineComponent
				if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlPawn->GetActorLocation(), CachedDestination)) {
					Spline->ClearSplinePoints();
					for (const FVector& point : NavPath->PathPoints) {
						Spline->AddSplinePoint(point, ESplineCoordinateSpace::World);
						// DrawDebugSphere(GetWorld(), point, 8.f, 8, FColor::Red, false, 5.f);
					}
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag GameplayTag)
{
	if (!GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB)) {
		if (GetAuraASC()) {
			GetAuraASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}

	if (bTargeting) {
		if (GetAuraASC()) {
			GetAuraASC()->AbilityInputTagHeld(GameplayTag);
		}
	}
	else {
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit)) {
			CachedDestination = Hit.ImpactPoint;
		}

		if (APawn* ControlPawn = GetPawn()) {
			const FVector WorldDirection = (CachedDestination - ControlPawn->GetActorLocation()).GetSafeNormal();
			ControlPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraASC()
{
	if (!AuraAbilitySystemComponent) {
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}