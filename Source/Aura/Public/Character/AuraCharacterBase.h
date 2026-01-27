// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"

#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;
class UMaterialInstance;
class UMaterialInstanceDynamic;
/*
	所有角色的基类（主角和敌人）
*/
UCLASS(Abstract)  // Abstract来控制这个类不能被实例化
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
protected:
	virtual void BeginPlay() override;

public:
	/*ICombatInterface*/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;

	virtual void Die() override;
	virtual bool isDead_Implementation() const override;
	virtual AActor * GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	/*ICombatInterface End*/

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	/*定义初始化ASC的方式，敌人是挂在Character，Aura是挂在PlayerState，所以需要子类自己去重写*/
	virtual void InitAbilityActorInfo();
	/* 给Character施加一个GameplayEffect */
	void ApplyEffectOnSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	/* 添加角色的默认能力 */
	void AddCharacterAbilities();
	/* 每个角色都可以指定默认需要施加的游戏效果，用于初始化角色的属性 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Attribute")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	/* 用上边设置的GameEffect初始化角色的属性 */
	virtual void InitializeDefaultAttributes() const;
	/* 一开始就需要赋予给角色的能力 */
	UPROPERTY(EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

protected:
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName = "LeftHandSocket";
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName = "RightHandSocket";
	
	
	/* 角色死亡时替换为消融材质 */
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/* 受击时播放的蒙太奇 */
	UPROPERTY(EditAnywhere, Category = "Combat|Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;
	/* 攻击时播放的蒙太奇(随机挑选一个) */
	UPROPERTY(EditAnywhere,Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

private:
	UFUNCTION(netmulticast, Reliable)
	virtual void MulticastHandleDeath();
};
