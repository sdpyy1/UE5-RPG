// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraEnhancedInputComponent.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class userClass, typename pressFuncType, typename releaseFuncType, typename HeldFuncType >
	void BindAbilityActions(const UAuraInputConfig* InputConfig, userClass* Object, pressFuncType pressFunc, releaseFuncType releaseFunc, HeldFuncType HeldFunc);
};

template<class userClass, typename pressFuncType, typename releaseFuncType, typename HeldFuncType>
void UAuraEnhancedInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, userClass* Object, pressFuncType pressFunc, releaseFuncType releaseFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction Action : InputConfig->AbilityInputActions) {
		if (Action.InputAction) {
			if (pressFunc) {
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, pressFunc, Action.InputTag);
			}
			if (releaseFunc) {
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, releaseFunc, Action.InputTag);
			}
			if (HeldFunc) {
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
