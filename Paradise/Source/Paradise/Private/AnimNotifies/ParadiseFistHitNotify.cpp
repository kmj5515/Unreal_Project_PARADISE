#include "AnimNotifies/ParadiseFistHitNotify.h"

#include "Characters/ParadiseSurvivalCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UParadiseFistHitNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	AParadiseSurvivalCharacter* Character = Cast<AParadiseSurvivalCharacter>(MeshComp->GetOwner());
	if (!Character)
	{
		return;
	}

	if (!Character->HasAuthority() && !Character->IsLocallyControlled())
	{
		return;
	}

	Character->ServerFistTraceAndApplyDamage();
}

