#pragma once

#include "CoreMinimal.h"
#include "Characters/ParadiseCharacterBase.h"
#include "Animation/AnimMontage.h"
#include "ParadiseSurvivalCharacter.generated.h"

class AParadiseWeaponBase;
class UGameplayEffect;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class PARADISE_API AParadiseSurvivalCharacter : public AParadiseCharacterBase
{
	GENERATED_BODY()
	
public:
	AParadiseSurvivalCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 복제 설정 (현재 장착 무기/슬롯)
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Enhanced Input: Mapping Context & Actions (BP에서 세팅)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Move;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Run;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_EquipSlot1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_EquipSlot2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Block;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Roll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Attack;

	// 무기 없을 때 사용할 주먹 공격 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* FistAttackMontage;

	// 맨손 데미지 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Fist")
	TSubclassOf<UGameplayEffect> FistDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Fist")
	float FistTraceDistance = 120.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Fist")
	float FistTraceRadius = 16.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Fist|Debug")
	bool bDebugFistTrace = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Fist|Debug", meta = (EditCondition = "bDebugFistTrace", ClampMin = "0.01"))
	float FistDebugDrawTime = 1.0f;

	// 간단한 무기 슬롯 인벤토리 (슬롯 인덱스로 접근)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Weapon")
	TArray<TSubclassOf<AParadiseWeaponBase>> WeaponSlots;

	// 현재 장착 중인 무기
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inventory|Weapon", Replicated)
	int32 CurrentWeaponSlotIndex = -1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inventory|Weapon", Replicated)
	AParadiseWeaponBase* CurrentWeapon = nullptr;

public:
	// 슬롯 장착 요청 (클라이언트 → 서버)
	UFUNCTION(Server, Reliable)
	void ServerEquipWeaponSlot(int32 SlotIndex);

	// 공격 요청 (클라이언트 → 서버)
	UFUNCTION(Server, Reliable)
	void ServerTryAttack();

	// AnimNotify에서 호출: 서버가 현재 근접 무기로 데미지 처리
	UFUNCTION(Server, Reliable)
	void ServerMeleeTraceAndApplyDamage();

	// AnimNotify에서 호출: 서버가 맨손 데미지 처리
	UFUNCTION(Server, Reliable)
	void ServerFistTraceAndApplyDamage();

	/** 서버에서만 호출: 공격 몽타주를 모든 클라이언트(및 리슨 서버 본인)에서 재생 */
	void PlayReplicatedAttackMontage(UAnimMontage* Montage, FName SectionName = NAME_None);

protected:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAttackMontage(UAnimMontage* Montage, FName SectionName);

	/** 공격 몽타주가 끝나기 전 연타 방지 (무기는 ShouldThrottleNewAttackInput 참고) */
	bool ShouldThrottleAttackInput() const;

protected:
	// 서버 내부에서만 사용하는 실제 장착/언장착 로직
	void EquipWeaponSlotInternal(int32 SlotIndex);
	void UnequipCurrentWeaponInternal();

protected:
	void Input_Look(const FInputActionValue& Value);
	void Input_Move(const FInputActionValue& Value);
	void Input_RunPressed(const FInputActionValue& Value);
	void Input_RunReleased(const FInputActionValue& Value);
	void Input_EquipSlot1(const FInputActionValue& Value);
	void Input_EquipSlot2(const FInputActionValue& Value);
	void Input_Attack(const FInputActionValue& Value);
	void Input_Block_Pressed(const FInputActionValue& Value);
	void Input_Block_Released(const FInputActionValue& Value);
	void Input_Roll(const FInputActionValue& Value);
	void Input_Jump_Pressed(const FInputActionValue& Value);
	void Input_Jump_Released(const FInputActionValue& Value);

	// 무기 없을 때 주먹 공격
	void PerformFistAttack();
	void PerformFistTraceAndApplyDamage();
};
