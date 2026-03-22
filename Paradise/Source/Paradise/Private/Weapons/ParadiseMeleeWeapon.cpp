// Simple melee weapon attack implementation

#include "Weapons/ParadiseMeleeWeapon.h"
#include "Characters/ParadiseSurvivalCharacter.h"
#include "Animation/AnimInstance.h"

bool AParadiseMeleeWeapon::ShouldThrottleNewAttackInput(const UAnimInstance* AnimInst) const
{
	if (bSupportsCombo && MaxComboCount > 1 && ComboSections.Num() > 0)
	{
		return false;
	}

	return Super::ShouldThrottleNewAttackInput(AnimInst);
}

void AParadiseMeleeWeapon::PerformAttack(AParadiseSurvivalCharacter* OwnerChar)
{
	if (!OwnerChar || !AttackMontage)
	{
		return;
	}

	// 콤보 미지원이면 1타만
	if (!bSupportsCombo || MaxComboCount <= 1 || ComboSections.Num() == 0)
	{
		OwnerChar->PlayReplicatedAttackMontage(AttackMontage, NAME_None);
		return;
	}

	// 간단한 순환 콤보
	CurrentComboIndex = (CurrentComboIndex + 1) % MaxComboCount;
	const FName SectionName = ComboSections.IsValidIndex(CurrentComboIndex)
		? ComboSections[CurrentComboIndex]
		: NAME_None;

	OwnerChar->PlayReplicatedAttackMontage(AttackMontage, SectionName);

	// TODO: AnimNotify에서 트레이스/데미지 호출
}

