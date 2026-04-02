// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "Characters/ParadiseCharacterBase.h"
#include "Interfaces/ParadiseWeaponHitReactable.h"
#include "GameplayEffectTypes.h"
#include "ParadiseEnemyCharacter.generated.h"

/**
 * 적(좀비 등)용 GAS 캐릭터 베이스.
 * 청각/시야 탐지, 추격, 근접 공격은 보통 AIController + Behavior Tree + Blackboard + Perception으로 구성합니다.
 */
UCLASS(Blueprintable)
class PARADISE_API AParadiseEnemyCharacter : public AParadiseCharacterBase, public IParadiseWeaponHitReactable
{
	GENERATED_BODY()

public:
	AParadiseEnemyCharacter();
	virtual void Tick(float DeltaSeconds) override;

	virtual void ReactToWeaponHit_Implementation(AActor* HitInstigator, const FHitResult& Hit, float HitStrength) override;
	
	UFUNCTION(Server, Reliable)
	void ServerEnemyAttackTraceAndApplyDamage();

	void TryPlayAttackMontage();
	bool IsDead() const { return bIsDead; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<class USoundBase> WeaponHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (ClampMin = "0.0"))
	float WeaponHitSoundVolumeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class UGameplayEffect> EnemyAttackDamageEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float EnemyAttackDistance = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float EnemyAttackRadius = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> EnemyAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> EnemyDieMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = "0.01"))
	float EnemyAttackCooldown = 1.2f;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsDead = false;

	float LastEnemyAttackTime = -1000.f;

	UFUNCTION()
	void OnRep_IsDead();

	void HandleDeath();
	void PerformEnemyAttackTraceAndApplyDamage();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
