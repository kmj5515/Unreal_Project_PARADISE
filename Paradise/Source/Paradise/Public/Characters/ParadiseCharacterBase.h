// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParadiseCharacterBase.generated.h"

//class UDirectAbilitySystemComponent;
//class UDirectAttributeSet;

UCLASS()
class PARADISE_API AParadiseCharacterBase : public ACharacter//, public IAbilitySystemInterface, public IPawnCombatInterface
{
	GENERATED_BODY()

public:
	AParadiseCharacterBase();

	//virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PossessedBy(AController* NewController) override;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UParadiseAbilitySystemComponent* ParadiseAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UParadiseAttributeSet* ParadiseAttributeSet;*/

public:
	//FORCEINLINE UParadiseAbilitySystemComponent* GetParadiseAbilitySystemComponent() const { return ParadiseAbilitySystemComponent; }

	//FORCEINLINE UParadiseAttributeSet* GetParadiseAttributeSet() const { return ParadiseAttributeSet; }
};
