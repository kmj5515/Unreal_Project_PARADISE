// Base weapon class for Paradise project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParadiseWeaponBase.generated.h"

class USkeletalMeshComponent;
class AParadiseCharacterBase;

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

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnEquipped(AParadiseCharacterBase* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnUnequipped();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};

