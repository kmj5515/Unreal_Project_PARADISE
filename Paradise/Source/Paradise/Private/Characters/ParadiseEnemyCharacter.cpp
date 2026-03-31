// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/ParadiseEnemyCharacter.h"
#include "AI/ParadiseEnemyAIController.h"
#include "AbilitySystem/ParadiseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundBase.h"

AParadiseEnemyCharacter::AParadiseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AParadiseEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bUseControllerDesiredRotation = false;
		MoveComp->bOrientRotationToMovement = true;
	}
}

void AParadiseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AParadiseEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsDead)
	{
		return;
	}

	const UParadiseAttributeSet* AttributeSet = GetParadiseAttributeSet();
	if (AttributeSet && AttributeSet->Health.GetCurrentValue() <= 0.f)
	{
		HandleDeath();
	}
}

void AParadiseEnemyCharacter::ReactToWeaponHit_Implementation(AActor* HitInstigator, const FHitResult& Hit, float HitStrength)
{
	if (WeaponHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponHitSound, Hit.ImpactPoint, GetActorRotation(), WeaponHitSoundVolumeMultiplier);
	}

	if (HasAuthority() && GetWorld())
	{
		UAISense_Hearing::ReportNoiseEvent(
			GetWorld(),
			Hit.ImpactPoint,
			1.f,
			HitInstigator ? HitInstigator : this,
			2000.f);
	}
}

void AParadiseEnemyCharacter::ServerEnemyAttackTraceAndApplyDamage_Implementation()
{
	if (bIsDead)
	{
		return;
	}

	PerformEnemyAttackTraceAndApplyDamage();
}

void AParadiseEnemyCharacter::TryPlayAttackMontage()
{
	if (!HasAuthority() || bIsDead || !EnemyAttackMontage || !GetWorld())
	{
		return;
	}

	const float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastEnemyAttackTime < EnemyAttackCooldown)
	{
		return;
	}

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
	{
		return;
	}

	UAnimInstance* Anim = MeshComp->GetAnimInstance();
	if (!Anim)
	{
		return;
	}

	if (Anim->Montage_IsPlaying(EnemyAttackMontage))
	{
		return;
	}

	Anim->Montage_Play(EnemyAttackMontage);
	LastEnemyAttackTime = Now;
}

void AParadiseEnemyCharacter::PerformEnemyAttackTraceAndApplyDamage()
{
	if (!HasAuthority() || bIsDead)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponent();
	if (!EnemyAttackDamageEffectClass || !SourceASC || !GetWorld())
	{
		return;
	}

	const FVector Start = GetActorLocation() + FVector(0.f, 0.f, 50.f);
	const FVector End = Start + GetActorForwardVector() * EnemyAttackDistance;

	TArray<FHitResult> Hits;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(EnemyAttackTrace), true);
	QueryParams.AddIgnoredActor(this);

	const bool bHit = GetWorld()->SweepMultiByChannel(
		Hits,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(EnemyAttackRadius),
		QueryParams);

	if (!bHit)
	{
		return;
	}

	TSet<TWeakObjectPtr<AActor>> HitActors;
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActors.Contains(HitActor))
		{
			continue;
		}
		HitActors.Add(HitActor);

		IAbilitySystemInterface* TargetInterface = Cast<IAbilitySystemInterface>(HitActor);
		if (!TargetInterface)
		{
			continue;
		}

		UAbilitySystemComponent* TargetASC = TargetInterface->GetAbilitySystemComponent();
		if (!TargetASC)
		{
			continue;
		}

		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		EffectContext.AddHitResult(Hit);

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(EnemyAttackDamageEffectClass, 1.f, EffectContext);
		if (!SpecHandle.IsValid() || !SpecHandle.Data.IsValid())
		{
			continue;
		}

		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AParadiseEnemyCharacter::OnRep_IsDead()
{
	if (AAIController* OwningAIController = Cast<AAIController>(GetController()))
	{
		OwningAIController->StopMovement();

		if (AParadiseEnemyAIController* EnemyAI = Cast<AParadiseEnemyAIController>(OwningAIController))
		{
			EnemyAI->OnControlledPawnDied();
		}
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->DisableMovement();
	}

	if (EnemyDieMontage)
	{
		if (USkeletalMeshComponent* MeshComp = GetMesh())
		{
			if (UAnimInstance* Anim = MeshComp->GetAnimInstance())
			{
				if (!Anim->Montage_IsPlaying(EnemyDieMontage))
				{
					Anim->Montage_Play(EnemyDieMontage);
				}
			}
		}
	}
}

void AParadiseEnemyCharacter::HandleDeath()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	OnRep_IsDead();
}

void AParadiseEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AParadiseEnemyCharacter, bIsDead);
}
