// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/ParadiseEnemyAIController.h"

#include "Characters/ParadiseEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISense_Hearing.h"

DEFINE_LOG_CATEGORY_STATIC(LogParadiseEnemyAI, Log, All);

AParadiseEnemyAIController::AParadiseEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	SetPerceptionComponent(*Perception);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 2000.f;
	HearingConfig->SetMaxAge(3.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	Perception->ConfigureSense(*HearingConfig);
	Perception->SetDominantSense(HearingConfig->GetSenseImplementation());
	Perception->OnPerceptionUpdated.AddDynamic(this, &AParadiseEnemyAIController::OnPerceptionUpdated);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AParadiseEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogParadiseEnemyAI, Warning, TEXT("[AI] OnPossess Pawn=%s Controller=%s"), *GetNameSafe(InPawn), *GetNameSafe(this));

	if (!EnemyBehaviorTree)
	{
		UE_LOG(LogParadiseEnemyAI, Error, TEXT("[AI] EnemyBehaviorTree is null. Set it in AIController defaults/BP."));
		return;
	}

	UBlackboardComponent* InitializedBlackboard = nullptr;
	if (!UseBlackboard(EnemyBehaviorTree->BlackboardAsset, InitializedBlackboard))
	{
		UE_LOG(LogParadiseEnemyAI, Error, TEXT("[AI] UseBlackboard failed. BT=%s BBAsset=%s"), *GetNameSafe(EnemyBehaviorTree), *GetNameSafe(EnemyBehaviorTree->BlackboardAsset));
		return;
	}

	RunBehaviorTree(EnemyBehaviorTree);
	UE_LOG(LogParadiseEnemyAI, Warning, TEXT("[AI] RunBehaviorTree success. BT=%s BB=%s"), *GetNameSafe(EnemyBehaviorTree), *GetNameSafe(InitializedBlackboard));
}

void AParadiseEnemyAIController::OnControlledPawnDied()
{
	StopMovement();

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->ClearValue(TargetActorKeyName);
	}

	if (Perception)
	{
		Perception->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
	}

	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("EnemyDead"));
	}
}

void AParadiseEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogParadiseEnemyAI, Warning, TEXT("[AI] OnPerceptionUpdated actors=%d"), UpdatedActors.Num());

	FVector BestLocation = FVector::ZeroVector;
	float BestAge = TNumericLimits<float>::Max();
	bool bFound = false;
	AActor* BestActor = nullptr;

	for (AActor* Actor : UpdatedActors)
	{
		if (!Actor)
		{
			continue;
		}

		APawn* AsPawn = Cast<APawn>(Actor);
		if (!AsPawn)
		{
			continue;
		}

		if (AsPawn == GetPawn())
		{
			continue;
		}

		AController* Ctr = AsPawn->GetController();
		if (!Ctr || !Ctr->IsPlayerController())
		{
			continue;
		}

		FActorPerceptionBlueprintInfo Info;
		Perception->GetActorsPerception(Actor, Info);

		for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			if (!Stimulus.WasSuccessfullySensed())
			{
				continue;
			}

			if (Stimulus.Type != UAISense::GetSenseID(UAISense_Hearing::StaticClass()))
			{
				continue;
			}

			const float Age = Stimulus.GetAge();
			if (Age < BestAge)
			{
				BestAge = Age;
				BestLocation = Stimulus.StimulusLocation;
				BestActor = Actor;
				bFound = true;
			}
		}
	}

	if (bFound)
	{
		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			BB->SetValueAsObject(TargetActorKeyName, BestActor);
			BB->SetValueAsVector(LastHeardLocationKeyName, BestLocation);
			UE_LOG(LogParadiseEnemyAI, Warning, TEXT("[AI] Hearing->BB updated. Target=%s Location=%s"), *GetNameSafe(BestActor), *BestLocation.ToString());
		}
	}
	else
	{
		UE_LOG(LogParadiseEnemyAI, Verbose, TEXT("[AI] PerceptionUpdated but no valid hearing stimulus."));
	}
}

