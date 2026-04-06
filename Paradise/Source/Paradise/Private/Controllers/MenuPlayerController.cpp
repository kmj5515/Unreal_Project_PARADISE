// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/MenuPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);

	// 로컬 UI는 권한과 무관하게 로컬 PC에서만 띄움 (리슨 서버/클라 모두)
	if (IsLocalPlayerController())
	{
		SpawnWidget();
	}
}

void AMenuPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (IsLocalPlayerController())
	{
		SpawnWidget();
	}
}

void AMenuPlayerController::SpawnWidget()
{
	if (MenuWidget || !MenuWidgetClass)
	{
		return;
	}

	MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);
	if (MenuWidget)
	{
		MenuWidget->AddToViewport();
	}
}
