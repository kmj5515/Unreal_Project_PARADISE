// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ParadiseInteractableActorBase.h"
#include "ParadiseBrokenCarInteractableActor.generated.h"

class UStaticMeshComponent;
class USoundBase;

UCLASS(Blueprintable)
class PARADISE_API AParadiseBrokenCarInteractableActor : public AParadiseInteractableActorBase
{
	GENERATED_BODY()

public:
	AParadiseBrokenCarInteractableActor();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CarMesh;

	/** 인터랙션 시 재생할 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> InteractSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (ClampMin = "0.0"))
	float VolumeMultiplier = 1.f;

	/** 한번만 재생할지 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	bool bPlayOnce = true;

	/** (서버 권한) 이미 재생했는지 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Sound")
	bool bHasPlayedSound = false;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayInteractSound();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

