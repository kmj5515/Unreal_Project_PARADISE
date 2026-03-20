// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ParadiseCharacterBase.h"
#include "AbilitySystem/ParadiseAbilitySystemComponent.h"
#include "AbilitySystem/ParadiseAttributeSet.h"

AParadiseCharacterBase::AParadiseCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ParadiseAbilitySystemComponent = CreateDefaultSubobject<UParadiseAbilitySystemComponent>(TEXT("ParadiseAbilitySystemComponent"));
	ParadiseAttributeSet = CreateDefaultSubobject<UParadiseAttributeSet>(TEXT("ParadiseAttributeSet"));
}

UAbilitySystemComponent* AParadiseCharacterBase::GetAbilitySystemComponent() const
{
	return ParadiseAbilitySystemComponent;
}

void AParadiseCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// ASC가 Pawn에 붙어있는 구조(현재 프로젝트는 PlayerState 기반 여부가 확실하지 않음)이므로,
	// BeginPlay에서 actor info를 초기화합니다.
	if (ParadiseAbilitySystemComponent)
	{
		ParadiseAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AParadiseCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ParadiseAbilitySystemComponent)
	{
		// 서버에서 먼저 초기화해두면 멀티플레이에서 디버깅이 편합니다.
		ParadiseAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}