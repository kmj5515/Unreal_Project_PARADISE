#include "AI/BTTasks/ParadiseBTTask_PlayEnemyAttack.h"

#include "AI/ParadiseEnemyAIController.h"
#include "Characters/ParadiseEnemyCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogParadiseEnemyBTTask, Log, All);

UParadiseBTTask_PlayEnemyAttack::UParadiseBTTask_PlayEnemyAttack()
{
	NodeName = TEXT("Play Enemy Attack");
}

EBTNodeResult::Type UParadiseBTTask_PlayEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AParadiseEnemyAIController* AIController = Cast<AParadiseEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		UE_LOG(LogParadiseEnemyBTTask, Error, TEXT("[BTTask] No AIController."));
		return EBTNodeResult::Failed;
	}

	AParadiseEnemyCharacter* Enemy = Cast<AParadiseEnemyCharacter>(AIController->GetPawn());
	if (!Enemy || Enemy->IsDead())
	{
		UE_LOG(LogParadiseEnemyBTTask, Warning, TEXT("[BTTask] Attack blocked. Enemy=%s Dead=%d"), *GetNameSafe(Enemy), Enemy ? (Enemy->IsDead() ? 1 : 0) : -1);
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogParadiseEnemyBTTask, Warning, TEXT("[BTTask] TryPlayAttackMontage Enemy=%s"), *GetNameSafe(Enemy));
	Enemy->TryPlayAttackMontage();
	return EBTNodeResult::Succeeded;
}
