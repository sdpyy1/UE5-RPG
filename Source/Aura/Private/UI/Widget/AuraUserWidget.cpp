// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget/AuraUserWidget.h"


/*
	给一个Widget设置Controller时，会触发WidgetControllerSet()事件，这个事件可以在一个Widget的蓝图中来监听
*/
void UAuraUserWidget::SetWidgetController(UObject* InController)
{
	this->WidgetController = InController;
	WidgetControllerSet();
}