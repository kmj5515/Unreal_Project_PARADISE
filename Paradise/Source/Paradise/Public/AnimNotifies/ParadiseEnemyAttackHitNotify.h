#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ParadiseEnemyAttackHitNotify.generated.h"

UCLASS(meta = (DisplayName = "Paradise Enemy Attack Hit Notify"))
class PARADISE_API UParadiseEnemyAttackHitNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

