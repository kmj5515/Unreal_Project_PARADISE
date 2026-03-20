// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "ParadiseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PARADISE_API UParadiseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UParadiseAttributeSet();

	// Health
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	// MaxHealth
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	// Attribute Accessor (GameplayEffect/GameplayAbility에서 사용)
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UParadiseAttributeSet, Health)
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UParadiseAttributeSet, MaxHealth)

	// GAS 기본 복제되는 값들
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;

protected:
	// Attribute 값 변경/이펙트 반영 시 클램프
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
