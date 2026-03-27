#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ParadiseMeleeHitNotify.generated.h"

/**
 * 근접 무기 공격 히트 프레임에 배치하는 노티파이.
 * 캐릭터의 ServerMeleeTraceAndApplyDamage를 호출합니다.
 */
UCLASS(meta = (DisplayName = "Paradise Melee Hit Notify"))
class PARADISE_API UParadiseMeleeHitNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

