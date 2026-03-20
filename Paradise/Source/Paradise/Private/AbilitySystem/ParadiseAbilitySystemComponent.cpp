// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ParadiseAbilitySystemComponent.h"

UParadiseAbilitySystemComponent::UParadiseAbilitySystemComponent()
{
	// GAS 기본 컴포넌트는 복제되어야 멀티 환경에서 Attribute 변화가 동기화됩니다.
	SetIsReplicatedByDefault(true);
}

