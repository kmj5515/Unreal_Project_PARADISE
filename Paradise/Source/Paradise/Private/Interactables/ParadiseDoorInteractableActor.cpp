// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/ParadiseDoorInteractableActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

AParadiseDoorInteractableActor::AParadiseDoorInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(SceneRoot);

	// 문은 상대 회전을 직접 복제(RepNotify+클라 애니메이션)하는 방식이라 이동 복제는 보통 필요 없습니다.
	SetReplicateMovement(false);
}

void AParadiseDoorInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	// 초기 “닫힘” 회전을 기준으로 열림/닫힘 목표를 계산합니다.
	if (DoorMesh)
	{
		ClosedRelativeRotation = DoorMesh->GetRelativeRotation();
	}

	// 서버에서는 초기 상태를 바로 반영합니다.
	if (HasAuthority())
	{
		const float YawDelta = bIsOpen ? OpenYawOffset : 0.f;
		if (DoorMesh)
		{
			DoorMesh->SetRelativeRotation(ClosedRelativeRotation + FRotator(0.f, YawDelta, 0.f));
		}
	}
}

void AParadiseDoorInteractableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsAnimating || !DoorMesh)
	{
		return;
	}

	const float Elapsed = GetWorld()->GetTimeSeconds() - AnimStartTime;
	const float Alpha = FMath::Clamp(RotationDuration <= 0.01f ? 1.f : (Elapsed / RotationDuration), 0.f, 1.f);
	const float SmoothAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);

	const FQuat StartQ = AnimStartRelativeRotation.Quaternion();
	const FQuat TargetQ = TargetRelativeRotation.Quaternion();
	const FQuat ResultQ = FQuat::Slerp(StartQ, TargetQ, SmoothAlpha);
	DoorMesh->SetRelativeRotation(ResultQ.Rotator());

	if (Alpha >= 1.f)
	{
		bIsAnimating = false;
	}
}

void AParadiseDoorInteractableActor::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);

	if (!HasAuthority())
	{
		return;
	}

	bIsOpen = !bIsOpen;
	OnRep_IsOpen();
}

void AParadiseDoorInteractableActor::OnRep_IsOpen()
{
	StartDoorAnimation(bIsOpen);
}

void AParadiseDoorInteractableActor::StartDoorAnimation(bool bOpen)
{
	if (!DoorMesh)
	{
		return;
	}

	bIsAnimating = true;
	AnimStartTime = GetWorld()->GetTimeSeconds();
	AnimStartRelativeRotation = DoorMesh->GetRelativeRotation();

	const float YawDelta = bOpen ? OpenYawOffset : 0.f;
	TargetRelativeRotation = ClosedRelativeRotation + FRotator(0.f, YawDelta, 0.f);
}

void AParadiseDoorInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AParadiseDoorInteractableActor, bIsOpen);
}

