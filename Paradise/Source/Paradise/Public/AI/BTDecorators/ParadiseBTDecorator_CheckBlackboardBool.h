#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "ParadiseBTDecorator_CheckBlackboardBool.generated.h"

UCLASS()
class PARADISE_API UParadiseBTDecorator_CheckBlackboardBool : public UBTDecorator
{
	GENERATED_BODY()

public:
	UParadiseBTDecorator_CheckBlackboardBool();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BoolKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bExpectedValue = true;
};
