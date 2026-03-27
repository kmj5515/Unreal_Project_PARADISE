// Simple melee weapon attack implementation

#include "Weapons/ParadiseMeleeWeapon.h"
#include "Characters/ParadiseSurvivalCharacter.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

bool AParadiseMeleeWeapon::ShouldThrottleNewAttackInput(const UAnimInstance* AnimInst) const
{
	if (bSupportsCombo && MaxComboCount > 1 && ComboSections.Num() > 0)
	{
		return false;
	}

	return Super::ShouldThrottleNewAttackInput(AnimInst);
}

void AParadiseMeleeWeapon::PerformAttack(AParadiseSurvivalCharacter* OwnerChar)
{
	if (!OwnerChar || !AttackMontage)
	{
		return;
	}

	// 콤보 미지원이면 1타만
	if (!bSupportsCombo || MaxComboCount <= 1 || ComboSections.Num() == 0)
	{
		OwnerChar->PlayReplicatedAttackMontage(AttackMontage, NAME_None);
		return;
	}

	// 간단한 순환 콤보
	CurrentComboIndex = (CurrentComboIndex + 1) % MaxComboCount;
	const FName SectionName = ComboSections.IsValidIndex(CurrentComboIndex)
		? ComboSections[CurrentComboIndex]
		: NAME_None;

	OwnerChar->PlayReplicatedAttackMontage(AttackMontage, SectionName);

	// TODO: AnimNotify에서 트레이스/데미지 호출
}

void AParadiseMeleeWeapon::MeleeTraceAndApplyDamage()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!DamageEffectClass || !OwningCharacter)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = OwningCharacter->GetAbilitySystemComponent();
	if (!SourceASC)
	{
		return;
	}

	const FVector Start = WeaponMesh ? WeaponMesh->GetComponentLocation() : OwningCharacter->GetActorLocation();
	const FVector End = Start + OwningCharacter->GetActorForwardVector() * TraceDistance;

	TArray<FHitResult> Hits;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(MeleeWeaponTrace), true);
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwningCharacter);

	const bool bHit = GetWorld() && GetWorld()->SweepMultiByChannel(
		Hits,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(TraceRadius),
		QueryParams);

	if (bDebugMeleeTrace && GetWorld())
	{
		const FColor TraceColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, TraceColor, false, DebugDrawTime, 0, 1.5f);
		DrawDebugSphere(GetWorld(), Start, TraceRadius, 12, FColor::Cyan, false, DebugDrawTime);
		DrawDebugSphere(GetWorld(), End, TraceRadius, 12, FColor::Cyan, false, DebugDrawTime);

		UE_LOG(LogTemp, Log, TEXT("[MeleeTrace] Hit=%s, HitsCount=%d, Start=%s, End=%s"),
			bHit ? TEXT("true") : TEXT("false"),
			Hits.Num(),
			*Start.ToCompactString(),
			*End.ToCompactString());
	}

	if (!bHit)
	{
		return;
	}

	TSet<TWeakObjectPtr<AActor>> HitActorsThisSwing;
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActor == OwningCharacter || HitActorsThisSwing.Contains(HitActor))
		{
			continue;
		}
		HitActorsThisSwing.Add(HitActor);

		IAbilitySystemInterface* TargetASCInterface = Cast<IAbilitySystemInterface>(HitActor);
		if (!TargetASCInterface)
		{
			continue;
		}

		UAbilitySystemComponent* TargetASC = TargetASCInterface->GetAbilitySystemComponent();
		if (!TargetASC)
		{
			continue;
		}

		if (bDebugMeleeTrace)
		{
			UE_LOG(LogTemp, Log, TEXT("[MeleeTrace] Apply damage to %s"), *HitActor->GetName());
			if (GetWorld())
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 8, FColor::Yellow, false, DebugDrawTime);
			}
		}

		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		EffectContext.AddHitResult(Hit);

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContext);
		if (!SpecHandle.IsValid() || !SpecHandle.Data.IsValid())
		{
			continue;
		}

		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

