// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/ParadiseSurvivalCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/ParadiseWeaponBase.h"
#include "Weapons/ParadiseMeleeWeapon.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"

AParadiseSurvivalCharacter::AParadiseSurvivalCharacter()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = 230.f;
	}
}

void AParadiseSurvivalCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AParadiseSurvivalCharacter, CurrentWeaponSlotIndex);
	DOREPLIFETIME(AParadiseSurvivalCharacter, CurrentWeapon);
}

void AParadiseSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DefaultMappingContext)
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Look)
		{
			EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AParadiseSurvivalCharacter::Input_Look);
		}

		if (IA_Move)
		{
			EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AParadiseSurvivalCharacter::Input_Move);
		}

		if (IA_Run)
		{
			// 누르고 있는 동안만 Run (500), 떼면 Walk (230)
			EnhancedInput->BindAction(IA_Run, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_RunPressed);
			EnhancedInput->BindAction(IA_Run, ETriggerEvent::Completed, this, &AParadiseSurvivalCharacter::Input_RunReleased);
		}

		if (IA_EquipSlot1)
		{
			EnhancedInput->BindAction(IA_EquipSlot1, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_EquipSlot1);
		}

		if (IA_EquipSlot2)
		{
			EnhancedInput->BindAction(IA_EquipSlot2, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_EquipSlot2);
		}

		if (IA_Attack)
		{
			EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_Attack);
		}

		if (IA_Block)
		{
			EnhancedInput->BindAction(IA_Block, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_Block_Pressed);
			EnhancedInput->BindAction(IA_Block, ETriggerEvent::Completed, this, &AParadiseSurvivalCharacter::Input_Block_Released);
		}

		if (IA_Roll)
		{
			EnhancedInput->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AParadiseSurvivalCharacter::Input_Roll);
		}

		if (IA_Jump)
		{
			EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_Jump_Pressed);
			EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AParadiseSurvivalCharacter::Input_Jump_Released);
		}
	}
}

void AParadiseSurvivalCharacter::Input_Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void AParadiseSurvivalCharacter::Input_Move(const FInputActionValue& Value)
{
	const FVector2D MoveAxis = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDir, MoveAxis.Y);
	AddMovementInput(RightDir, MoveAxis.X);
}

void AParadiseSurvivalCharacter::Input_RunPressed(const FInputActionValue& Value)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = 500.f;
	}
}

void AParadiseSurvivalCharacter::Input_RunReleased(const FInputActionValue& Value)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = 230.f;
	}
}

void AParadiseSurvivalCharacter::Input_EquipSlot1(const FInputActionValue& Value)
{
	ServerEquipWeaponSlot(0);
}

void AParadiseSurvivalCharacter::Input_EquipSlot2(const FInputActionValue& Value)
{
	ServerEquipWeaponSlot(1);
}

void AParadiseSurvivalCharacter::ServerEquipWeaponSlot_Implementation(int32 SlotIndex)
{
	EquipWeaponSlotInternal(SlotIndex);
}

void AParadiseSurvivalCharacter::EquipWeaponSlotInternal(int32 SlotIndex)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!WeaponSlots.IsValidIndex(SlotIndex))
	{
		return;
	}

	TSubclassOf<AParadiseWeaponBase> WeaponClass = WeaponSlots[SlotIndex];
	if (!*WeaponClass)
	{
		return;
	}

	// 같은 슬롯 다시 누른 경우: 나중에 토글(집어넣기) 로직을 넣을 수 있음
	if (CurrentWeaponSlotIndex == SlotIndex && CurrentWeapon)
	{
		return;
	}

	// 기존 무기 언장착
	UnequipCurrentWeaponInternal();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParadiseWeaponBase* NewWeapon = World->SpawnActor<AParadiseWeaponBase>(WeaponClass, SpawnParams);
	if (!NewWeapon)
	{
		return;
	}

	NewWeapon->OnEquipped(this);

	CurrentWeapon = NewWeapon;
	CurrentWeaponSlotIndex = SlotIndex;

	// TODO: 여기서 장착 몽타주(무기 꺼내기)를 재생하고 싶으면 AnimInstance에 Notify를 추가
}

void AParadiseSurvivalCharacter::UnequipCurrentWeaponInternal()
{
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->OnUnequipped();
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	CurrentWeaponSlotIndex = -1;
}

void AParadiseSurvivalCharacter::Input_Attack(const FInputActionValue& Value)
{
	ServerTryAttack();
}

bool AParadiseSurvivalCharacter::ShouldThrottleAttackInput() const
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
	{
		return false;
	}

	const UAnimInstance* Anim = MeshComp->GetAnimInstance();
	if (!Anim)
	{
		return false;
	}

	if (CurrentWeapon)
	{
		return CurrentWeapon->ShouldThrottleNewAttackInput(Anim);
	}

	if (FistAttackMontage)
	{
		return Anim->Montage_IsPlaying(FistAttackMontage);
	}

	return false;
}

void AParadiseSurvivalCharacter::PlayReplicatedAttackMontage(UAnimMontage* Montage, FName SectionName)
{
	if (!Montage || !HasAuthority())
	{
		return;
	}

	MulticastPlayAttackMontage(Montage, SectionName);
}

void AParadiseSurvivalCharacter::MulticastPlayAttackMontage_Implementation(UAnimMontage* Montage, FName SectionName)
{
	if (!Montage)
	{
		return;
	}

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
	{
		return;
	}

	if (UAnimInstance* Anim = MeshComp->GetAnimInstance())
	{
		Anim->Montage_Play(Montage);
		if (SectionName != NAME_None)
		{
			Anim->Montage_JumpToSection(SectionName, Montage);
		}
	}
}

void AParadiseSurvivalCharacter::ServerTryAttack_Implementation()
{
	// TODO: 상태 체크(죽음, 스턴, 쿨타임 등) 필요하면 여기서

	if (ShouldThrottleAttackInput())
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->PerformAttack(this);
	}
	else
	{
		PerformFistAttack();
	}
}

void AParadiseSurvivalCharacter::ServerMeleeTraceAndApplyDamage_Implementation()
{
	if (!CurrentWeapon)
	{
		return;
	}

	AParadiseMeleeWeapon* MeleeWeapon = Cast<AParadiseMeleeWeapon>(CurrentWeapon);
	if (!MeleeWeapon)
	{
		return;
	}

	MeleeWeapon->MeleeTraceAndApplyDamage();
}

void AParadiseSurvivalCharacter::ServerFistTraceAndApplyDamage_Implementation()
{
	PerformFistTraceAndApplyDamage();
}

void AParadiseSurvivalCharacter::PerformFistAttack()
{
	if (!FistAttackMontage)
	{
		return;
	}

	PlayReplicatedAttackMontage(FistAttackMontage, NAME_None);

	// TODO: AnimNotify로 주먹 히트 트레이스/데미지 처리
}

void AParadiseSurvivalCharacter::PerformFistTraceAndApplyDamage()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!FistDamageEffectClass)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponent();
	if (!SourceASC)
	{
		return;
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	const FVector Start = EyeLocation;
	const FVector End = Start + EyeRotation.Vector() * FistTraceDistance;

	TArray<FHitResult> Hits;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(FistTrace), true);
	QueryParams.AddIgnoredActor(this);

	const bool bHit = GetWorld() && GetWorld()->SweepMultiByChannel(
		Hits,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(FistTraceRadius),
		QueryParams);

	if (bDebugFistTrace && GetWorld())
	{
		const FColor TraceColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, TraceColor, false, FistDebugDrawTime, 0, 1.5f);
		DrawDebugSphere(GetWorld(), Start, FistTraceRadius, 12, FColor::Cyan, false, FistDebugDrawTime);
		DrawDebugSphere(GetWorld(), End, FistTraceRadius, 12, FColor::Cyan, false, FistDebugDrawTime);

		UE_LOG(LogTemp, Log, TEXT("[FistTrace] Hit=%s, HitsCount=%d, Start=%s, End=%s"),
			bHit ? TEXT("true") : TEXT("false"),
			Hits.Num(),
			*Start.ToCompactString(),
			*End.ToCompactString());
	}

	if (!bHit)
	{
		return;
	}

	TSet<TWeakObjectPtr<AActor>> HitActorsThisPunch;
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActor == this || HitActorsThisPunch.Contains(HitActor))
		{
			continue;
		}
		HitActorsThisPunch.Add(HitActor);

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

		if (bDebugFistTrace)
		{
			UE_LOG(LogTemp, Log, TEXT("[FistTrace] Apply damage to %s"), *HitActor->GetName());
			if (GetWorld())
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 8, FColor::Yellow, false, FistDebugDrawTime);
			}
		}

		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		EffectContext.AddHitResult(Hit);

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(FistDamageEffectClass, 1.f, EffectContext);
		if (!SpecHandle.IsValid() || !SpecHandle.Data.IsValid())
		{
			continue;
		}

		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AParadiseSurvivalCharacter::Input_Block_Pressed(const FInputActionValue& Value)
{
	// TODO: Block 활성화 로직
}

void AParadiseSurvivalCharacter::Input_Block_Released(const FInputActionValue& Value)
{
	// TODO: Block 비활성화 로직
}

void AParadiseSurvivalCharacter::Input_Roll(const FInputActionValue& Value)
{
	// TODO: 구르기(Roll) 로직
}

void AParadiseSurvivalCharacter::Input_Jump_Pressed(const FInputActionValue& Value)
{
	Jump();
}

void AParadiseSurvivalCharacter::Input_Jump_Released(const FInputActionValue& Value)
{
	StopJumping();
}

