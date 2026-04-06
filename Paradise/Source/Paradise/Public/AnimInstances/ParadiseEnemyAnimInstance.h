// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/ParadiseBaseAnimInstance.h"
#include "ParadiseEnemyAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

/**
 * 적 캐릭터용 AnimInstance. 지상 이동 속도 등을 노출해 Blend Space / State Machine에 연결합니다.
 */
UCLASS()
class PARADISE_API UParadiseEnemyAnimInstance : public UParadiseBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AnimData|Status")
	float GetEnemyHealth() const;

protected:
	UPROPERTY()
	ACharacter* OwningCharacter = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|Locomotion")
	float GroundSpeed = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|Locomotion")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|Locomotion")
	bool IsFalling = false;
};
