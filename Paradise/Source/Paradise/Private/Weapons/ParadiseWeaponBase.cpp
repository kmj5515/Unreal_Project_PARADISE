// Base weapon implementation

#include "Weapons/ParadiseWeaponBase.h"
#include "Characters/ParadiseCharacterBase.h"
#include "Characters/ParadiseSurvivalCharacter.h"
#include "Animation/AnimInstance.h"

AParadiseWeaponBase::AParadiseWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	AttachSocketName = TEXT("Weapon_R");

	bReplicates = true;
	SetReplicateMovement(true);
}

void AParadiseWeaponBase::OnEquipped(AParadiseCharacterBase* NewOwner)
{
	OwningCharacter = NewOwner;

	if (OwningCharacter && WeaponMesh)
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		WeaponMesh->AttachToComponent(OwningCharacter->GetMesh(), AttachRules, AttachSocketName);
	}
}

void AParadiseWeaponBase::OnUnequipped()
{
	if (WeaponMesh)
	{
		WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	OwningCharacter = nullptr;
}

bool AParadiseWeaponBase::ShouldThrottleNewAttackInput(const UAnimInstance* AnimInst) const
{
	if (!AnimInst || !AttackMontage)
	{
		return false;
	}

	return AnimInst->Montage_IsPlaying(AttackMontage);
}

void AParadiseWeaponBase::PerformAttack(AParadiseSurvivalCharacter* OwnerChar)
{
	if (!OwnerChar || !AttackMontage)
	{
		return;
	}

	OwnerChar->PlayReplicatedAttackMontage(AttackMontage, NAME_None);
}

