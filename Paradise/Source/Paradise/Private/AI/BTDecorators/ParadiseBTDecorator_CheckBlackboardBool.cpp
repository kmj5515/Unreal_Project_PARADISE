#include "AI/BTDecorators/ParadiseBTDecorator_CheckBlackboardBool.h"

#include "BehaviorTree/BlackboardComponent.h"

UParadiseBTDecorator_CheckBlackboardBool::UParadiseBTDecorator_CheckBlackboardBool()
{
	NodeName = TEXT("Check Blackboard Bool");
}

bool UParadiseBTDecorator_CheckBlackboardBool::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BoolKey.SelectedKeyName.IsValid())
	{
		return false;
	}

	const bool bValue = BB->GetValueAsBool(BoolKey.SelectedKeyName);
	return bValue == bExpectedValue;
}
