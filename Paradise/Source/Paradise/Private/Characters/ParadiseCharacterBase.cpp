// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParadiseCharacterBase.h"

AParadiseCharacterBase::AParadiseCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//ParadiseAbilitySystemComponent = CreateDefaultSubobject<UParadiseAbilitySystemComponent>(TEXT("ParadiseAbilitySystemComponent"));

	//ParadiseAttributeSet = CreateDefaultSubobject<UParadiseAttributeSet>(TEXT("ParadiseAttributeSet"));
}

//UAbilitySystemComponent* AParadiseCharacterBase::GetAbilitySystemComponent() const
//{
//	return GetParadiseAbilitySystemComponent();
//}

void AParadiseCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}