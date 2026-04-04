// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnStartGameClicked);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnQuitGameClicked);
	}
}

void UMainMenuUserWidget::OnStartGameClicked()
{
	if (LobbyLevelName.IsNone())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UGameplayStatics::OpenLevel(World, LobbyLevelName, true, TEXT("listen"));
}

void UMainMenuUserWidget::OnQuitGameClicked()
{
	APlayerController* PC = GetOwningPlayer();
	UWorld* World = GetWorld();
	if (!World || !PC)
	{
		return;
	}

	UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, false);
}
