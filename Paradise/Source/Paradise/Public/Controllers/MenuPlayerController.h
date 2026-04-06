// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PARADISE_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;

protected:
	/** 로비/메인메뉴 등 — C++ 자식 클래스 기본값에서도 에디터에 노출되도록 protected */
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MenuWidget;

	void SpawnWidget();
};
