// Simple ranged weapon (e.g., pistol/rifle)

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ParadiseWeaponBase.h"
#include "ParadiseRangedWeapon.generated.h"

UCLASS()
class PARADISE_API AParadiseRangedWeapon : public AParadiseWeaponBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon|Ranged")
	virtual void Fire();
};

