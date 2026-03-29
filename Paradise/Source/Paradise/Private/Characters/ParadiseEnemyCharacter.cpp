// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/ParadiseEnemyCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

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
