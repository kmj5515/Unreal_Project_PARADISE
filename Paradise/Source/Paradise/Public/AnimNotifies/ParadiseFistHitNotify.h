#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ParadiseFistHitNotify.generated.h"

/**
 * 맨손 공격 히트 프레임에 배치하는 노티파이.
 * 캐릭터의 ServerFistTraceAndApplyDamage를 호출합니다.
 */
UCLASS(meta = (DisplayName = "Paradise Fist Hit Notify"))
class PARADISE_API UParadiseFistHitNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

