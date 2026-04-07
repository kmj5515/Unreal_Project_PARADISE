// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/LobbyShopUserWidget.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Engine/DataTable.h"
#include "Widgets/LobbyShopItemEntryWidget.h"

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

	PopulateShopItems();
}

void ULobbyShopUserWidget::RefreshShopItems()
{
	PopulateShopItems();
}

void ULobbyShopUserWidget::PopulateShopItems()
{
	if (!ItemListPanel || !ShopItemsDataTable || !ShopItemEntryWidgetClass)
	{
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	ItemListPanel->ClearChildren();

	TArray<FShopItemDataRow*> ItemRows;
	ShopItemsDataTable->GetAllRows(TEXT("LobbyShopItems"), ItemRows);

	for (const FShopItemDataRow* ItemRow : ItemRows)
	{
		if (!ItemRow)
		{
			continue;
		}

		ULobbyShopItemEntryWidget* ItemEntry = CreateWidget<ULobbyShopItemEntryWidget>(PC, ShopItemEntryWidgetClass);
		if (!ItemEntry)
		{
			continue;
		}

		ItemEntry->SetItemData(*ItemRow);
		ItemListPanel->AddChild(ItemEntry);
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
