// Simple ranged weapon (e.g., pistol/rifle)

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ParadiseWeaponBase.h"
#include "ParadiseRangedWeapon.generated.h"

class UGameplayEffect;

UCLASS()
class PARADISE_API AParadiseRangedWeapon : public AParadiseWeaponBase
{
	GENERATED_BODY()

public:
	virtual void PerformAttack(class AParadiseSurvivalCharacter* OwnerChar) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ranged")
	void Fire();

	// 히트된 대상의 ASC에 적용할 데미지 GameplayEffect 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ranged|Debug")
	bool bDebugWeaponHitReact = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ranged|Debug", meta = (EditCondition = "bDebugWeaponHitReact", ClampMin = "0.01"))
	float WeaponHitReactDebugDrawTime = 1.0f;
};

