// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ParadiseInteractable.h"
#include "ParadiseInteractableActorBase.generated.h"

/**
 * 인터랙션 오브젝트용 베이스 액터.
 * - 이 클래스를 상속받으면 `Paradise Interactable` 인터페이스 구현이 자동으로 포함됩니다.
 * - 블루프린트에서는 `BP_OnInteract`만 오버라이드해서 로직을 작성하면 됩니다.
 */
UCLASS(Abstract, Blueprintable)
class PARADISE_API AParadiseInteractableActorBase : public AActor, public IParadiseInteractable
{
	GENERATED_BODY()

public:
	AParadiseInteractableActorBase();

	/** 서버(또는 호출자)에서 인터랙션이 발생하면 호출됩니다. */
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	/** 인터랙션 처리 로직을 블루프린트에서 작성하세요. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BP_OnInteract(AActor* Interactor);
};

