// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/LobbyShopUserWidget.h"
#include "Components/Button.h"

void ULobbyShopUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseShopButton)
	{
		CloseShopButton->OnClicked.AddDynamic(this, &ULobbyShopUserWidget::OnCloseClicked);
	}

	if (BackToLobbyButton)
	{
		BackToLobbyButton->OnClicked.AddDynamic(this, &ULobbyShopUserWidget::OnBackToLobbyClicked);
	}
}

void ULobbyShopUserWidget::OnCloseClicked()
{
	RemoveFromParent();
}

void ULobbyShopUserWidget::OnBackToLobbyClicked()
{
	RemoveFromParent();
}
