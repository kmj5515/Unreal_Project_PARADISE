// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ParadiseEnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Hearing;
class UBehaviorTree;
class UBehaviorTreeComponent;

UCLASS()
class PARADISE_API AParadiseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AParadiseEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	void OnControlledPawnDied();
	float GetAttackTriggerDistance() const { return AttackTriggerDistance; }
	FName GetTargetActorKeyName() const { return TargetActorKeyName; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> Perception;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Combat")
	float AttackTriggerDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Behavior")
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Behavior")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Blackboard")
	FName TargetActorKeyName = TEXT("TargetActor");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Blackboard")
	FName LastHeardLocationKeyName = TEXT("LastHeardLocation");

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};

