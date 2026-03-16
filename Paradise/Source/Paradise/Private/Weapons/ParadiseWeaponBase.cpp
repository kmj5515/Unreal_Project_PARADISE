// Base weapon implementation

#include "Weapons/ParadiseWeaponBase.h"
#include "Characters/ParadiseCharacterBase.h"

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

