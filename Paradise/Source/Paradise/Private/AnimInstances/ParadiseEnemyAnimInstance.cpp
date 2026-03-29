// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstances/ParadiseEnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UParadiseEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* Owner = GetOwningActor();
	OwningCharacter = Cast<ACharacter>(Owner);

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
	else
	{
		OwningMovementComponent = nullptr;
	}
}

void UParadiseEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter)
	{
		AActor* Owner = GetOwningActor();
		OwningCharacter = Cast<ACharacter>(Owner);

		if (OwningCharacter)
		{
			OwningMovementComponent = OwningCharacter->GetCharacterMovement();
		}
	}

	if (!OwningMovementComponent)
	{
		GroundSpeed = 0.f;
		Velocity = FVector::ZeroVector;
		IsFalling = false;
		return;
	}

	Velocity = OwningMovementComponent->Velocity;
	const FVector LateralVelocity(Velocity.X, Velocity.Y, 0.f);
	GroundSpeed = LateralVelocity.Size();
	IsFalling = OwningMovementComponent->IsFalling();
}
