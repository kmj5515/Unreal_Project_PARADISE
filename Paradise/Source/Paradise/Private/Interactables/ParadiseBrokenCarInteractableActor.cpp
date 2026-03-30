// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/ParadiseBrokenCarInteractableActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundBase.h"

AParadiseBrokenCarInteractableActor::AParadiseBrokenCarInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(false);

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	CarMesh->SetupAttachment(SceneRoot);
}

void AParadiseBrokenCarInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AParadiseBrokenCarInteractableActor::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);

	if (!HasAuthority())
	{
		return;
	}

	if (bPlayOnce && bHasPlayedSound)
	{
		return;
	}

	if (InteractSound)
	{
		bHasPlayedSound = true;
		MulticastPlayInteractSound();
	}
}

void AParadiseBrokenCarInteractableActor::MulticastPlayInteractSound_Implementation()
{
	if (!InteractSound)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation(), GetActorRotation(), VolumeMultiplier);
}

void AParadiseBrokenCarInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AParadiseBrokenCarInteractableActor, bHasPlayedSound);
}

