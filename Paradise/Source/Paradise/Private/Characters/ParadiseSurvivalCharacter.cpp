// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/ParadiseSurvivalCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

void AParadiseSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DefaultMappingContext)
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Look)
		{
			EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AParadiseSurvivalCharacter::Input_Look);
		}

		if (IA_Move)
		{
			EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AParadiseSurvivalCharacter::Input_Move);
		}

		if (IA_Block)
		{
			EnhancedInput->BindAction(IA_Block, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_Block_Pressed);
			EnhancedInput->BindAction(IA_Block, ETriggerEvent::Completed, this, &AParadiseSurvivalCharacter::Input_Block_Released);
		}

		if (IA_Roll)
		{
			EnhancedInput->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AParadiseSurvivalCharacter::Input_Roll);
		}

		if (IA_Jump)
		{
			EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_Jump_Pressed);
			EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AParadiseSurvivalCharacter::Input_Jump_Released);
		}
	}
}

void AParadiseSurvivalCharacter::Input_Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void AParadiseSurvivalCharacter::Input_Move(const FInputActionValue& Value)
{
	const FVector2D MoveAxis = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDir, MoveAxis.Y);
	AddMovementInput(RightDir, MoveAxis.X);
}

void AParadiseSurvivalCharacter::Input_Block_Pressed(const FInputActionValue& Value)
{
	// TODO: Block 활성화 로직
}

void AParadiseSurvivalCharacter::Input_Block_Released(const FInputActionValue& Value)
{
	// TODO: Block 비활성화 로직
}

void AParadiseSurvivalCharacter::Input_Roll(const FInputActionValue& Value)
{
	// TODO: 구르기(Roll) 로직
}

void AParadiseSurvivalCharacter::Input_Jump_Pressed(const FInputActionValue& Value)
{
	Jump();
}

void AParadiseSurvivalCharacter::Input_Jump_Released(const FInputActionValue& Value)
{
	StopJumping();
}

