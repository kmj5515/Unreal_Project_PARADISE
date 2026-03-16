// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/ParadiseCharacterAnimInstance.h"
#include "Characters/ParadiseSurvivalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UParadiseCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* Owner = GetOwningActor();
	OwningCharacter = Cast<AParadiseSurvivalCharacter>(Owner);

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
	else
	{
		OwningMovementComponent = nullptr;
	}
}

void UParadiseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter)
	{
		AActor* Owner = GetOwningActor();
		OwningCharacter = Cast<AParadiseSurvivalCharacter>(Owner);

		if (OwningCharacter)
		{
			OwningMovementComponent = OwningCharacter->GetCharacterMovement();
		}
	}

	if (!OwningMovementComponent)
	{
		return;
	}

	Velocity = OwningMovementComponent->Velocity;
	const FVector LateralVelocity(Velocity.X, Velocity.Y, 0.f);

	GroundSpeed = LateralVelocity.Size();
	IsFalling = OwningMovementComponent->IsFalling();

	const FVector Accel = OwningMovementComponent->GetCurrentAcceleration();
	ShouldMove = GroundSpeed > KINDA_SMALL_NUMBER && !Accel.IsNearlyZero();
}
