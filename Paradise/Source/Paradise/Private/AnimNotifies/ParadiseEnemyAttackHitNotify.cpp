#include "AnimNotifies/ParadiseEnemyAttackHitNotify.h"

#include "Characters/ParadiseEnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UParadiseEnemyAttackHitNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	AParadiseEnemyCharacter* Enemy = Cast<AParadiseEnemyCharacter>(MeshComp->GetOwner());
	if (!Enemy)
	{
		return;
	}

	if (Enemy->HasAuthority())
	{
		Enemy->ServerEnemyAttackTraceAndApplyDamage();
	}
}

