#include "AI/BTServices/ParadiseBTService_UpdateEnemyCombatState.h"

#include "AI/ParadiseEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/ParadiseEnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogParadiseEnemyBTService, Log, All);

UParadiseBTService_UpdateEnemyCombatState::UParadiseBTService_UpdateEnemyCombatState()
{
	NodeName = TEXT("Update Enemy Combat State");
	Interval = 0.1f;
}

void UParadiseBTService_UpdateEnemyCombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AParadiseEnemyAIController* AIController = Cast<AParadiseEnemyAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BB)
	{
		return;
	}

	AParadiseEnemyCharacter* Enemy = Cast<AParadiseEnemyCharacter>(AIController->GetPawn());
	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Enemy || Enemy->IsDead() || !TargetActor)
	{
		BB->SetValueAsBool(ShouldAttackKey.SelectedKeyName, false);
		UE_LOG(LogParadiseEnemyBTService, Verbose, TEXT("[BTService] ShouldAttack=false (Enemy=%s Dead=%d Target=%s)"),
			*GetNameSafe(Enemy),
			Enemy ? (Enemy->IsDead() ? 1 : 0) : -1,
			*GetNameSafe(TargetActor));
		return;
	}

	const float DistSq = FVector::DistSquared(Enemy->GetActorLocation(), TargetActor->GetActorLocation());
	const bool bShouldAttack = DistSq <= FMath::Square(AIController->GetAttackTriggerDistance());
	BB->SetValueAsBool(ShouldAttackKey.SelectedKeyName, bShouldAttack);
	UE_LOG(LogParadiseEnemyBTService, Warning, TEXT("[BTService] Dist=%.1f ShouldAttack=%d Target=%s"),
		FMath::Sqrt(DistSq),
		bShouldAttack ? 1 : 0,
		*GetNameSafe(TargetActor));

	if (bShouldAttack)
	{
		AIController->StopMovement();
	}
	else
	{
		const EPathFollowingRequestResult::Type MoveResult =
			AIController->MoveToActor(TargetActor, AIController->GetAttackTriggerDistance() * 0.8f);
		UE_LOG(LogParadiseEnemyBTService, Warning, TEXT("[BTService] MoveToActor result=%d Target=%s"),
			static_cast<int32>(MoveResult),
			*GetNameSafe(TargetActor));
	}
}
