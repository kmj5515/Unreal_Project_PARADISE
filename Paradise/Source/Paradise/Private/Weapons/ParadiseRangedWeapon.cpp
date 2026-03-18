// Simple ranged weapon fire implementation

#include "Weapons/ParadiseRangedWeapon.h"
#include "Characters/ParadiseSurvivalCharacter.h"
#include "Animation/AnimInstance.h"

void AParadiseRangedWeapon::PerformAttack(AParadiseSurvivalCharacter* OwnerChar)
{
	if (!OwnerChar)
	{
		return;
	}

	// 사격용 몽타주가 있으면 재생
	if (AttackMontage)
	{
		if (UAnimInstance* Anim = OwnerChar->GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(AttackMontage);
		}
	}

	Fire();
}

void AParadiseRangedWeapon::Fire()
{
	// TODO: Implement hit-scan or projectile spawning and apply damage
}

