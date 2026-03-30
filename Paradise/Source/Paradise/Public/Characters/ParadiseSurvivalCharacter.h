#pragma once

#include "CoreMinimal.h"
#include "Characters/ParadiseCharacterBase.h"
#include "Animation/AnimMontage.h"
#include "Engine/EngineTypes.h"
#include "ParadiseSurvivalCharacter.generated.h"

class AParadiseWeaponBase;
class UGameplayEffect;
class UUserWidget;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnParadiseInteractableFocusChanged, AActor*, FocusedActor);

UCLASS()
class PARADISE_API AParadiseSurvivalCharacter : public AParadiseCharacterBase
{
	GENERATED_BODY()
	
public:
	AParadiseSurvivalCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyControllerChanged() override;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_UnequipWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Interact;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Trace")
	float InteractionTraceDistance = 350.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Trace")
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel = ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Debug")
	bool bDebugInteractionTrace = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Debug", meta = (EditCondition = "bDebugInteractionTrace", ClampMin = "0.01"))
	float InteractionDebugDrawTime = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|UI")
	TSubclassOf<UUserWidget> InteractionPromptWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|UI")
	float InteractionPromptWidgetOffsetY = 20.f;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Fist|Debug")
	bool bDebugFistWeaponHitReact = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Fist|Debug", meta = (EditCondition = "bDebugFistTrace || bDebugFistWeaponHitReact", ClampMin = "0.01"))
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

	UFUNCTION(Server, Reliable)
	void ServerUnequipWeapon();

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

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetFocusedInteractable() const;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnParadiseInteractableFocusChanged OnInteractableFocusChanged;

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* Target);

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
	void Input_UnequipWeapon(const FInputActionValue& Value);
	void Input_Attack(const FInputActionValue& Value);
	void Input_Block_Pressed(const FInputActionValue& Value);
	void Input_Block_Released(const FInputActionValue& Value);
	void Input_Roll(const FInputActionValue& Value);
	void Input_Jump_Pressed(const FInputActionValue& Value);
	void Input_Jump_Released(const FInputActionValue& Value);
	void Input_Interact(const FInputActionValue& Value);

	void UpdateInteractionFocus();
	void SetInteractableFocus(AActor* NewFocus);
	void EnsureInteractionPromptWidget();
	void RefreshInteractionPromptVisibility();
	bool ValidateInteractTarget(AActor* Target) const;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> FocusedInteractable;

	UPROPERTY(Transient)
	FVector FocusedInteractableImpactPoint = FVector::ZeroVector;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> InteractionPromptWidget;

	// 무기 없을 때 주먹 공격
	void PerformFistAttack();
	void PerformFistTraceAndApplyDamage();
};
