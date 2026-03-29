// Simple melee weapon (e.g., dagger, hammer)

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ParadiseWeaponBase.h"
#include "ParadiseMeleeWeapon.generated.h"

class UGameplayEffect;

UCLASS()
class PARADISE_API AParadiseMeleeWeapon : public AParadiseWeaponBase
{
	GENERATED_BODY()

public:
	virtual void PerformAttack(class AParadiseSurvivalCharacter* OwnerChar) override;
	virtual bool ShouldThrottleNewAttackInput(const UAnimInstance* AnimInst) const override;

	// AnimNotify 시점에 서버에서 호출: 근접 트레이스 + GAS 데미지 적용
	UFUNCTION(BlueprintCallable, Category = "Weapon|Melee|Damage")
	void MeleeTraceAndApplyDamage();

	// 히트된 대상의 ASC에 적용할 데미지 GameplayEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Melee|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Melee|Trace")
	float TraceDistance = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Melee|Trace")
	float TraceRadius = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Melee|Debug")
	bool bDebugMeleeTrace = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Melee|Debug")
	bool bDebugWeaponHitReact = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Melee|Debug", meta = (EditCondition = "bDebugMeleeTrace || bDebugWeaponHitReact", ClampMin = "0.01"))
	float DebugDrawTime = 1.0f;
};

