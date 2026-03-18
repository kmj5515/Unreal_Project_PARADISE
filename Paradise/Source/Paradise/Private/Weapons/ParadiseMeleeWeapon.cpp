// Simple melee weapon attack implementation

#include "Weapons/ParadiseMeleeWeapon.h"
#include "Characters/ParadiseSurvivalCharacter.h"
#include "Animation/AnimInstance.h"

void AParadiseMeleeWeapon::PerformAttack(AParadiseSurvivalCharacter* OwnerChar)
{
	if (!OwnerChar || !AttackMontage)
	{
		return;
	}

	// 콤보 미지원이면 1타만
	if (!bSupportsCombo || MaxComboCount <= 1 || ComboSections.Num() == 0)
	{
		if (UAnimInstance* Anim = OwnerChar->GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(AttackMontage);
		}
		return;
	}

	// 간단한 순환 콤보
	CurrentComboIndex = (CurrentComboIndex + 1) % MaxComboCount;
	const FName SectionName = ComboSections.IsValidIndex(CurrentComboIndex)
		? ComboSections[CurrentComboIndex]
		: NAME_None;

	if (UAnimInstance* Anim = OwnerChar->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(AttackMontage);
		if (SectionName != NAME_None)
		{
			Anim->Montage_JumpToSection(SectionName, AttackMontage);
		}
	}

	// TODO: AnimNotify에서 트레이스/데미지 호출
}

