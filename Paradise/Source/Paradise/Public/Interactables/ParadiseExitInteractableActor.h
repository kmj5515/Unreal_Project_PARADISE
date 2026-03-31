// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ParadiseInteractableActorBase.h"
#include "ParadiseExitInteractableActor.generated.h"

/**
 * 탈출구 인터랙션 액터.
 * - 플레이어가 상호작용하면 해당 플레이어만 지정된 레벨로 이동(ClientTravel) 시도.
 */
UCLASS(Blueprintable)
class PARADISE_API AParadiseExitInteractableActor : public AParadiseInteractableActorBase
{
	GENERATED_BODY()

public:
	AParadiseExitInteractableActor();

	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ExitMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
	TSoftObjectPtr<UWorld> TargetLevel;
};
