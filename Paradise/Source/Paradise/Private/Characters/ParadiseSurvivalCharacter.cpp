// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/ParadiseSurvivalCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/ParadiseInteractable.h"
#include "Weapons/ParadiseWeaponBase.h"
#include "Weapons/ParadiseMeleeWeapon.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/ParadiseWeaponHitReactable.h"

namespace ParadiseInteractionTrace
{
static TArray<FHitResult> GatherSortedHits(
	UWorld* World,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel Channel,
	const FCollisionQueryParams& Params)
{
	TArray<FHitResult> Hits;
	if (!World)
	{
		return Hits;
	}
	World->LineTraceMultiByChannel(Hits, Start, End, Channel, Params);
	if (Hits.Num() == 0)
	{
		return Hits;
	}
	Hits.Sort([](const FHitResult& A, const FHitResult& B) { return A.Distance < B.Distance; });
	return Hits;
}

static AActor* ResolveInteractableFromHit(const FHitResult& Hit)
{
	if (AActor* A = Hit.GetActor())
	{
		if (A->GetClass()->ImplementsInterface(UParadiseInteractable::StaticClass()))
		{
			return A;
		}
	}
	if (UPrimitiveComponent* Comp = Hit.GetComponent())
	{
		if (AActor* OwnerActor = Comp->GetOwner())
		{
			if (OwnerActor->GetClass()->ImplementsInterface(UParadiseInteractable::StaticClass()))
			{
				return OwnerActor;
			}
		}
	}
	return nullptr;
}

static AActor* FindFirstInteractableAlongRay(
	UWorld* World,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel Channel,
	const FCollisionQueryParams& Params)
{
	for (const FHitResult& Hit : GatherSortedHits(World, Start, End, Channel, Params))
	{
		if (AActor* Found = ResolveInteractableFromHit(Hit))
		{
			return Found;
		}
	}
	return nullptr;
}

static bool FindFirstInteractableAlongRayHit(
	UWorld* World,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel Channel,
	const FCollisionQueryParams& Params,
	AActor*& OutActor,
	FVector& OutImpactPoint)
{
	OutActor = nullptr;
	OutImpactPoint = FVector::ZeroVector;

	for (const FHitResult& Hit : GatherSortedHits(World, Start, End, Channel, Params))
	{
		if (AActor* Found = ResolveInteractableFromHit(Hit))
		{
			OutActor = Found;
			OutImpactPoint = Hit.ImpactPoint;
			return true;
		}
	}
	return false;
}

static bool IsTargetAlongInteractRay(
	UWorld* World,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel Channel,
	const FCollisionQueryParams& Params,
	AActor* Target)
{
	if (!Target)
	{
		return false;
	}
	for (const FHitResult& Hit : GatherSortedHits(World, Start, End, Channel, Params))
	{
		if (Hit.GetActor() == Target)
		{
			return true;
		}
		if (UPrimitiveComponent* Comp = Hit.GetComponent())
		{
			if (Comp->GetOwner() == Target)
			{
				return true;
			}
		}
	}
	return false;
}
}

AParadiseSurvivalCharacter::AParadiseSurvivalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = WalkSpeed;
	}
}

void AParadiseSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(IsLocallyControlled());
}

void AParadiseSurvivalCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	SetActorTickEnabled(IsLocallyControlled());
}

void AParadiseSurvivalCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!IsLocallyControlled())
	{
		return;
	}
	UpdateInteractionFocus();
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

		if (IA_UnequipWeapon)
		{
			EnhancedInput->BindAction(IA_UnequipWeapon, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_UnequipWeapon);
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

		if (IA_Interact)
		{
			EnhancedInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &AParadiseSurvivalCharacter::Input_Interact);
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
		// 즉각 반응을 위해 로컬에서도 먼저 반영(오토노머스 프록시 예측)
		MoveComp->MaxWalkSpeed = RunSpeed;
	}
	ServerSetRunning(true);
}

void AParadiseSurvivalCharacter::Input_RunReleased(const FInputActionValue& Value)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = WalkSpeed;
	}
	ServerSetRunning(false);
}

void AParadiseSurvivalCharacter::ServerSetRunning_Implementation(bool bNewRunning)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = bNewRunning ? RunSpeed : WalkSpeed;
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

void AParadiseSurvivalCharacter::Input_UnequipWeapon(const FInputActionValue& Value)
{
	ServerUnequipWeapon();
}

void AParadiseSurvivalCharacter::ServerUnequipWeapon_Implementation()
{
	if (!CurrentWeapon)
	{
		return;
	}
	UnequipCurrentWeaponInternal();
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

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponent();

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

		if (HitActor->GetClass()->ImplementsInterface(UParadiseWeaponHitReactable::StaticClass()))
		{
			constexpr float HitStrength = 1.f;
			ParadiseLogWeaponHitReactDebug(GetWorld(), bDebugFistWeaponHitReact, FistDebugDrawTime, HitActor, Hit, this, HitStrength, TEXT("Fist"));
			IParadiseWeaponHitReactable::Execute_ReactToWeaponHit(HitActor, this, Hit, HitStrength);
		}

		if (!FistDamageEffectClass || !SourceASC)
		{
			continue;
		}

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

AActor* AParadiseSurvivalCharacter::GetFocusedInteractable() const
{
	return FocusedInteractable.Get();
}

void AParadiseSurvivalCharacter::Input_Interact(const FInputActionValue& Value)
{
	AActor* Target = FocusedInteractable.Get();
	if (!Target)
	{
		return;
	}
	ServerInteract(Target);
}

void AParadiseSurvivalCharacter::ServerInteract_Implementation(AActor* Target)
{
	if (!HasAuthority() || !IsValid(Target))
	{
		return;
	}
	if (!Target->GetClass()->ImplementsInterface(UParadiseInteractable::StaticClass()))
	{
		return;
	}
	if (!ValidateInteractTarget(Target))
	{
		return;
	}
	IParadiseInteractable::Execute_Interact(Target, this);
}

bool AParadiseSurvivalCharacter::ValidateInteractTarget(AActor* Target) const
{
	if (!Target || !GetWorld())
	{
		return false;
	}

	FVector TraceStart;
	FRotator UnusedRot;
	GetActorEyesViewPoint(TraceStart, UnusedRot);
	const FVector TraceEnd = TraceStart + GetActorForwardVector() * InteractionTraceDistance;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionValidate), true);
	Params.AddIgnoredActor(this);

	if (ParadiseInteractionTrace::IsTargetAlongInteractRay(
			GetWorld(),
			TraceStart,
			TraceEnd,
			InteractionTraceChannel,
			Params,
			Target))
	{
		return true;
	}

	return false;
}

void AParadiseSurvivalCharacter::UpdateInteractionFocus()
{
	FVector Start;
	FRotator UnusedRot;
	GetActorEyesViewPoint(Start, UnusedRot);
	const FVector End = Start + GetActorForwardVector() * InteractionTraceDistance;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionFocus), true);
	Params.AddIgnoredActor(this);

	AActor* NewFocus = nullptr;
	FVector NewImpactPoint = FVector::ZeroVector;
	ParadiseInteractionTrace::FindFirstInteractableAlongRayHit(
		GetWorld(),
		Start,
		End,
		InteractionTraceChannel,
		Params,
		NewFocus,
		NewImpactPoint);

	FocusedInteractableImpactPoint = NewImpactPoint;

	if (bDebugInteractionTrace && GetWorld())
	{
		TArray<FHitResult> DebugHits;
		GetWorld()->LineTraceMultiByChannel(DebugHits, Start, End, InteractionTraceChannel, Params);
		const bool bAnyHit = DebugHits.Num() > 0;
		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			bAnyHit ? FColor::Green : FColor::Red,
			false,
			InteractionDebugDrawTime,
			0,
			1.5f);
		for (const FHitResult& DH : DebugHits)
		{
			DrawDebugSphere(GetWorld(), DH.ImpactPoint, 6.f, 8, FColor::Yellow, false, InteractionDebugDrawTime, 0, 1.f);
		}
		if (NewFocus)
		{
			DrawDebugString(GetWorld(), End, FString::Printf(TEXT("Focus: %s"), *NewFocus->GetName()), nullptr, FColor::Cyan, InteractionDebugDrawTime);
		}
	}

	SetInteractableFocus(NewFocus);

	if (NewFocus && InteractionPromptWidgetClass)
	{
		EnsureInteractionPromptWidget();
		if (InteractionPromptWidget)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			if (PC)
			{
				FVector2D WidgetPos;
				if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, NewImpactPoint, WidgetPos, false))
				{
					WidgetPos.Y -= InteractionPromptWidgetOffsetY;
					InteractionPromptWidget->SetPositionInViewport(WidgetPos, false);
				}
			}
		}
	}
}

void AParadiseSurvivalCharacter::SetInteractableFocus(AActor* NewFocus)
{
	AActor* Previous = FocusedInteractable.Get();
	if (Previous == NewFocus)
	{
		return;
	}

	FocusedInteractable = NewFocus;
	OnInteractableFocusChanged.Broadcast(NewFocus);
	RefreshInteractionPromptVisibility();
}

void AParadiseSurvivalCharacter::EnsureInteractionPromptWidget()
{
	if (InteractionPromptWidget || !InteractionPromptWidgetClass)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	InteractionPromptWidget = CreateWidget<UUserWidget>(PC, InteractionPromptWidgetClass);
}

void AParadiseSurvivalCharacter::RefreshInteractionPromptVisibility()
{
	if (!InteractionPromptWidgetClass)
	{
		return;
	}

	EnsureInteractionPromptWidget();
	if (!InteractionPromptWidget)
	{
		return;
	}

	if (FocusedInteractable.IsValid())
	{
		if (!InteractionPromptWidget->IsInViewport())
		{
			InteractionPromptWidget->AddToViewport();
		}
		InteractionPromptWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractionPromptWidget->SetVisibility(ESlateVisibility::Collapsed);
		if (InteractionPromptWidget->IsInViewport())
		{
			InteractionPromptWidget->RemoveFromParent();
		}
	}
}

