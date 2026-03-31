// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ParadiseExitInteractableActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AParadiseExitInteractableActor::AParadiseExitInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	ExitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExitMesh"));
	ExitMesh->SetupAttachment(SceneRoot);
}

void AParadiseExitInteractableActor::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);

	if (!HasAuthority() || !Interactor)
	{
		return;
	}

	APawn* Pawn = Cast<APawn>(Interactor);
	APlayerController* PC = Pawn ? Cast<APlayerController>(Pawn->GetController()) : nullptr;
	if (!PC)
	{
		return;
	}

	if (!TargetLevel.ToSoftObjectPath().IsValid())
	{
		return;
	}

	if (!TargetLevel.IsValid())
	{
		TargetLevel.LoadSynchronous();
	}

	UWorld* TargetWorld = TargetLevel.Get();
	if (!TargetWorld)
	{
		return;
	}

	const FString PackageName = TargetWorld->GetOutermost()->GetName();
	if (PackageName.IsEmpty())
	{
		return;
	}

	PC->ClientTravel(PackageName, ETravelType::TRAVEL_Absolute);
}

