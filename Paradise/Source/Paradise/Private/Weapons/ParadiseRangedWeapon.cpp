// Simple ranged weapon fire implementation

#include "Weapons/ParadiseRangedWeapon.h"
#include "Characters/ParadiseSurvivalCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Engine/World.h"

void AParadiseRangedWeapon::PerformAttack(AParadiseSurvivalCharacter* OwnerChar)
{
	if (!OwnerChar)
	{
		return;
	}

	// 사격용 몽타주가 있으면 모든 클라이언트에서 재생
	if (AttackMontage)
	{
		OwnerChar->PlayReplicatedAttackMontage(AttackMontage, NAME_None);
	}

	Fire();
}

void AParadiseRangedWeapon::Fire()
{
	if (!DamageEffectClass)
	{
		return;
	}

	if (!OwningCharacter)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = OwningCharacter->GetAbilitySystemComponent();
	if (!SourceASC)
	{
		return;
	}

	// 간단 히트스캔 (현재는 시야 기준으로 발사)
	FVector EyeLocation;
	FRotator EyeRotation;
	OwningCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	const FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000.f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(WeaponTrace), true);
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwningCharacter);

	if (!GetWorld() || !GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
	{
		return;
	}

	IAbilitySystemInterface* TargetASCInterface = Cast<IAbilitySystemInterface>(HitActor);
	if (!TargetASCInterface)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = TargetASCInterface->GetAbilitySystemComponent();
	if (!TargetASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	EffectContext.AddHitResult(HitResult);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContext);
	if (!SpecHandle.IsValid() || !SpecHandle.Data.IsValid())
	{
		return;
	}

	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

