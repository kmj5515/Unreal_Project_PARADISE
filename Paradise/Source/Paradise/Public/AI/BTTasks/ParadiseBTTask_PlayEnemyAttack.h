#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ParadiseBTTask_PlayEnemyAttack.generated.h"

UCLASS()
class PARADISE_API UParadiseBTTask_PlayEnemyAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UParadiseBTTask_PlayEnemyAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
