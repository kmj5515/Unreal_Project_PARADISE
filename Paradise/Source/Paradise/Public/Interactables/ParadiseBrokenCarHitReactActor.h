// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ParadiseWeaponHitReactable.h"
#include "ParadiseBrokenCarHitReactActor.generated.h"

class UAudioComponent;
class UStaticMeshComponent;
class USoundBase;

/**
 * 공격(타격) 시 사이렌을 켜는 "망가진 차" 액터.
 * - 무기 히트 인터페이스로 호출됨
 * - 멀티: 서버에서 bSirenOn을 복제하고, 각 클라에서 오디오를 켬
 */
UCLASS(Blueprintable)
class PARADISE_API AParadiseBrokenCarHitReactActor : public AActor, public IParadiseWeaponHitReactable
{
	GENERATED_BODY()

public:
	AParadiseBrokenCarHitReactActor();

	virtual void BeginPlay() override;

	virtual void ReactToWeaponHit_Implementation(AActor* HitInstigator, const FHitResult& Hit, float HitStrength) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CarMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAudioComponent> SirenAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Siren")
	TObjectPtr<USoundBase> SirenSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Siren", meta = (ClampMin = "0.0"))
	float SirenVolumeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Siren")
	bool bToggleSirenOnHit = true;

	UPROPERTY(ReplicatedUsing = OnRep_SirenOn, BlueprintReadOnly, Category = "Siren")
	bool bSirenOn = false;

	UFUNCTION()
	void OnRep_SirenOn();

	void ApplySirenState();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

