#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "ParadiseBTService_UpdateEnemyCombatState.generated.h"

UCLASS()
class PARADISE_API UParadiseBTService_UpdateEnemyCombatState : public UBTService
{
	GENERATED_BODY()

public:
	UParadiseBTService_UpdateEnemyCombatState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ShouldAttackKey;
};
