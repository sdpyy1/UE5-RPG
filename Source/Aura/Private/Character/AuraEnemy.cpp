// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraEnemy.h"
#include <AbilitySystem/AuraAbilitySystemComponent.h>
#include <AbilitySystem/AuraAttributeSet.h>
#include "Components/WidgetComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibary.h"
#include "UI/Widget/AuraUserWidget.h"
AAuraEnemy::AAuraEnemy()
{
	//Cursor Trace In Visibility Channel For Enemy HighLight In PlayerController
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	/////////////////////////////  GAS /////////////////////////////
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(250);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	// 这样蓝图就能获得Controller了（就是这个Character本身）
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject())) {
		AuraUserWidget->SetWidgetController(this);
	}

	// Bind Callback To ACS
	if (UAuraAttributeSet* AuraASC = CastChecked<UAuraAttributeSet>(AttributeSet)) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraASC->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnHealthChangedDelegate.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraASC->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
			OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
			});

		OnHealthChangedDelegate.Broadcast(AuraASC->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AuraASC->GetMaxHealth());
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibary::InitDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::InitAbilityActorInfo()
{
	// 1. 组件的拥有者是谁 2. 技能作用于谁（比如是谁发射技能）
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityCharacterInfoSet();

	InitializeDefaultAttributes();
}