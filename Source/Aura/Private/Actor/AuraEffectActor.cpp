// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AuraEffectActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <AbilitySystemInterface.h>

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* overlapComponent, AActor* OtherActor, UPrimitiveComponent* otherComponent, int32 OtherBodyIndex, bool bFromSwap, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASIF = Cast<IAbilitySystemInterface>(OtherActor)) {
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(ASIF->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));

		// Const-Cast For Now
		UAuraAttributeSet* nonConstAuraAttribeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		nonConstAuraAttribeSet->SetHealth(nonConstAuraAttribeSet->GetHealth() + 30.f);
		Destroy();
	}
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
}