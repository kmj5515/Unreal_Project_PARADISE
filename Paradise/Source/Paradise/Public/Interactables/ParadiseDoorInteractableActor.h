// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ParadiseInteractableActorBase.h"
#include "ParadiseDoorInteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class PARADISE_API AParadiseDoorInteractableActor : public AParadiseInteractableActorBase
{
	GENERATED_BODY()

public:
	AParadiseDoorInteractableActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// 인터랙션 시 문을 열/닫습니다(토글).
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	/** 닫힘 상태(기본 회전) - BeginPlay에서 DoorMesh의 현재 relative rotation을 저장합니다. */
	FRotator ClosedRelativeRotation;

	/** 열림 yaw offset (relative Z 축 기준). 예: 90이면 시계/반시계는 메시에 따라 달라집니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float OpenYawOffset = 90.f;

	/** 회전 시간(초). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door", meta = (ClampMin = "0.01"))
	float RotationDuration = 0.5f;

	/** 열림 여부(복제). */
	UPROPERTY(ReplicatedUsing = OnRep_IsOpen, BlueprintReadOnly, Category = "Door")
	bool bIsOpen = false;

	UFUNCTION()
	void OnRep_IsOpen();

	void StartDoorAnimation(bool bOpen);

	/** 애니메이션용. */
	bool bIsAnimating = false;
	float AnimStartTime = 0.f;
	FRotator AnimStartRelativeRotation;
	FRotator TargetRelativeRotation;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

