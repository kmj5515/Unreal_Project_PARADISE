// Base weapon class for Paradise project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParadiseWeaponBase.generated.h"

class USkeletalMeshComponent;
class AParadiseCharacterBase;
class UAnimInstance;

UCLASS()
class PARADISE_API AParadiseWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AParadiseWeaponBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName AttachSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AParadiseCharacterBase* OwningCharacter;

	// ===== 공격 공통 설정 =====
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attack")
	bool bSupportsCombo = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attack", meta = (EditCondition = "bSupportsCombo"))
	int32 MaxComboCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attack")
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attack", meta = (EditCondition = "bSupportsCombo"))
	TArray<FName> ComboSections;

	int32 CurrentComboIndex = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnEquipped(AParadiseCharacterBase* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnUnequipped();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	// 공통 공격 진입점
	UFUNCTION(BlueprintCallable, Category = "Weapon|Attack")
	virtual void PerformAttack(class AParadiseSurvivalCharacter* OwnerChar);

	/** true면 같은 공격 몽타주가 끝나기 전 새 공격 입력 무시 (콤보 무기는 오버라이드로 예외 처리 가능) */
	virtual bool ShouldThrottleNewAttackInput(const UAnimInstance* AnimInst) const;
};

