// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ParadiseInteractable.generated.h"

class AActor;

UINTERFACE(MinimalAPI, Blueprintable)
class UParadiseInteractable : public UInterface
{
	GENERATED_BODY()
};

class IParadiseInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Instigator);
};
