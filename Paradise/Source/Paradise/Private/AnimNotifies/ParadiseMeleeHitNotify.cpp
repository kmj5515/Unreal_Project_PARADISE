#include "AnimNotifies/ParadiseMeleeHitNotify.h"

#include "Characters/ParadiseSurvivalCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UParadiseMeleeHitNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	// 멀티캐스트 몽타주로 다른 클라에서도 Notify가 돌면, 비소유자가 Server RPC를 내면 안 됨.
	if (!Character->HasAuthority() && !Character->IsLocallyControlled())
	{
		return;
	}

	Character->ServerMeleeTraceAndApplyDamage();
}

