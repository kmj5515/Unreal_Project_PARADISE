#pragma once

#include "CoreMinimal.h"
#include "Characters/ParadiseCharacterBase.h"
#include "ParadiseSurvivalCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class PARADISE_API AParadiseSurvivalCharacter : public AParadiseCharacterBase
{
	GENERATED_BODY()
	
public:
	AParadiseSurvivalCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Enhanced Input: Mapping Context & Actions (BP에서 세팅)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Move;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Run;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Block;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Roll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Jump;

protected:
	void Input_Look(const FInputActionValue& Value);
	void Input_Move(const FInputActionValue& Value);
	void Input_RunPressed(const FInputActionValue& Value);
	void Input_RunReleased(const FInputActionValue& Value);
	void Input_Block_Pressed(const FInputActionValue& Value);
	void Input_Block_Released(const FInputActionValue& Value);
	void Input_Roll(const FInputActionValue& Value);
	void Input_Jump_Pressed(const FInputActionValue& Value);
	void Input_Jump_Released(const FInputActionValue& Value);
};
