// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"

UAuraOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UAuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class is nullptr"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller need initialized"));

	// 根据蓝图中设置的OverlayWidget类来创建一个Widget
	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UAuraOverlayWidgetController* Controller = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(Controller);
	Controller->BroadcastInitialValues();
	widget->AddToViewport();
}