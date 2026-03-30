// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/ParadiseBrokenCarHitReactActor.h"

#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundBase.h"

AParadiseBrokenCarHitReactActor::AParadiseBrokenCarHitReactActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(false);

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	CarMesh->SetupAttachment(SceneRoot);

	SirenAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SirenAudio"));
	SirenAudio->SetupAttachment(SceneRoot);
	SirenAudio->bAutoActivate = false;
	SirenAudio->bIsUISound = false;
}

void AParadiseBrokenCarHitReactActor::BeginPlay()
{
	Super::BeginPlay();

	if (SirenAudio && SirenSound)
	{
		SirenAudio->SetSound(SirenSound);
		SirenAudio->SetVolumeMultiplier(SirenVolumeMultiplier);
	}

	ApplySirenState();
}

void AParadiseBrokenCarHitReactActor::ReactToWeaponHit_Implementation(AActor* HitInstigator, const FHitResult& Hit, float HitStrength)
{
	if (!HasAuthority())
	{
		return;
	}

	if (bToggleSirenOnHit)
	{
		bSirenOn = !bSirenOn;
	}
	else
	{
		bSirenOn = true;
	}

	OnRep_SirenOn();
}

void AParadiseBrokenCarHitReactActor::OnRep_SirenOn()
{
	ApplySirenState();
}

void AParadiseBrokenCarHitReactActor::ApplySirenState()
{
	if (!SirenAudio)
	{
		return;
	}

	if (bSirenOn)
	{
		if (!SirenAudio->IsPlaying())
		{
			SirenAudio->Play();
		}
	}
	else
	{
		if (SirenAudio->IsPlaying())
		{
			SirenAudio->Stop();
		}
	}
}

void AParadiseBrokenCarHitReactActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AParadiseBrokenCarHitReactActor, bSirenOn);
}

