// Simple melee weapon (e.g., dagger, hammer)

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ParadiseWeaponBase.h"
#include "ParadiseMeleeWeapon.generated.h"

UCLASS()
class PARADISE_API AParadiseMeleeWeapon : public AParadiseWeaponBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon|Melee")
	virtual void PerformAttack();
};

