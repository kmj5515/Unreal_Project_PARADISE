// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/ParadiseEnemyCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AParadiseEnemyCharacter::AParadiseEnemyCharacter()
{
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bUseControllerDesiredRotation = false;
		MoveComp->bOrientRotationToMovement = true;
	}
}

void AParadiseEnemyCharacter::ReactToWeaponHit_Implementation(AActor* HitInstigator, const FHitResult& Hit, float HitStrength)
{
	if (WeaponHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponHitSound, Hit.ImpactPoint, GetActorRotation(), WeaponHitSoundVolumeMultiplier);
	}
}
