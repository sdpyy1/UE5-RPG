// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectileActor.generated.h"
class USphereComponent;
class UPrimitiveComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UAudioComponent;
UCLASS()
class AURA_API AAuraProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectileActor();
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	bool bHit = false;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
	UPROPERTY()
	TObjectPtr<UAudioComponent> loopSoundComponent;
};
