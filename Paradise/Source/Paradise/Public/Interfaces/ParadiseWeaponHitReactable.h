// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ParadiseWeaponHitReactable.generated.h"

class AActor;
class UWorld;
struct FHitResult;

/**
 * 무기 트레이스/히트스캔에 맞은 맵 오브젝트(문, 차량 등)가 서버에서 반응할 때 구현합니다.
 * GAS 데미지와 별개로 호출됩니다(둘 다 구현 가능).
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UParadiseWeaponHitReactable : public UInterface
{
	GENERATED_BODY()
};

class IParadiseWeaponHitReactable
{
	GENERATED_BODY()

public:
	/** Instigator: 공격한 캐릭터 등. HitStrength: 추후 무기 데미지 수치와 연동 가능(현재는 1.f 고정 호출). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void ReactToWeaponHit(AActor* Instigator, const FHitResult& Hit, float HitStrength);
};

/** `ReactToWeaponHit` 호출 직전에 로그 + 임팩트 스피어(서버에서만 호출하는 것을 권장). */
PARADISE_API void ParadiseLogWeaponHitReactDebug(
	UWorld* World,
	bool bEnabled,
	float DrawDuration,
	AActor* HitActor,
	const FHitResult& Hit,
	AActor* Instigator,
	float HitStrength,
	const TCHAR* SourceLabel);
