// Fill out your copyright notice in the Description page of Project Settings.

#include "Interfaces/ParadiseWeaponHitReactable.h"
#include "DrawDebugHelpers.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"

void ParadiseLogWeaponHitReactDebug(
	UWorld* World,
	bool bEnabled,
	float DrawDuration,
	AActor* HitActor,
	const FHitResult& Hit,
	AActor* Instigator,
	float HitStrength,
	const TCHAR* SourceLabel)
{
	if (!bEnabled || !World)
	{
		return;
	}

	const FString HitName = HitActor ? HitActor->GetName() : FString(TEXT("(null)"));
	const FString InstName = Instigator ? Instigator->GetName() : FString(TEXT("(null)"));
	UE_LOG(LogTemp, Log, TEXT("[WeaponHitReact|%s] Instigator=%s -> HitActor=%s Impact=%s Comp=%s Strength=%.2f"),
		SourceLabel ? SourceLabel : TEXT("?"),
		*InstName,
		*HitName,
		*Hit.ImpactPoint.ToCompactString(),
		Hit.Component.IsValid() ? *Hit.Component->GetName() : TEXT("(none)"),
		HitStrength);

	DrawDebugSphere(World, Hit.ImpactPoint, 14.f, 10, FColor::Orange, false, DrawDuration, 0, 2.f);
}
