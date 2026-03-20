// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ParadiseAttributeSet.h"
#include "Net/UnrealNetwork.h"

// GAMEPLAYATTRIBUTE_REPNOTIFY 매크로/이펙트 콜백에서 사용하는 타입들
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

UParadiseAttributeSet::UParadiseAttributeSet()
{
	// 기본 스탯 (테스트용). 나중에 데이터/밸런스로 분리하는 것을 권장합니다.
	MaxHealth.SetBaseValue(100.f);
	MaxHealth.SetCurrentValue(100.f);

	Health.SetBaseValue(100.f);
	Health.SetCurrentValue(100.f);
}

void UParadiseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UParadiseAttributeSet, Health, OldHealth);
}

void UParadiseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UParadiseAttributeSet, MaxHealth, OldMaxHealth);
}

void UParadiseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, MaxHealth.GetCurrentValue());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}

	Super::PreAttributeChange(Attribute, NewValue);
}

void UParadiseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Health가 이펙트로 변경된 직후 클램프.
		const float NewHealth = FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue());
		Health.SetCurrentValue(NewHealth);

		UE_LOG(LogTemp, Log, TEXT("Paradise Health changed: %f"), NewHealth);
	}
}

void UParadiseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UParadiseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UParadiseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}
