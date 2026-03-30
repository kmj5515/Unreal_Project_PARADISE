// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/ParadiseInteractableActorBase.h"

AParadiseInteractableActorBase::AParadiseInteractableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AParadiseInteractableActorBase::Interact_Implementation(AActor* Interactor)
{
	BP_OnInteract(Interactor);
}

