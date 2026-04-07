// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/LobbyUserWidget.h"
#include "Widgets/LobbyShopUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void ULobbyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreateGameButton)
	{
		CreateGameButton->OnClicked.AddDynamic(this, &ULobbyUserWidget::OnCreateGameClicked);
	}

	if (JoinGameButton)
	{
		JoinGameButton->OnClicked.AddDynamic(this, &ULobbyUserWidget::OnJoinGameClicked);
	}

	if (OpenShopButton)
	{
		OpenShopButton->OnClicked.AddDynamic(this, &ULobbyUserWidget::OnOpenShopClicked);
	}
}

void ULobbyUserWidget::OnCreateGameClicked()
{
	if (GameplayLevelName.IsNone())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UGameplayStatics::OpenLevel(World, GameplayLevelName, true, TEXT("listen"));
}

void ULobbyUserWidget::OnJoinGameClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	FString Address = DefaultJoinAddress;
	if (JoinAddressField)
	{
		const FString Typed = JoinAddressField->GetText().ToString().TrimStartAndEnd();
		if (!Typed.IsEmpty())
		{
			Address = Typed;
		}
	}

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void ULobbyUserWidget::OnOpenShopClicked()
{
	if (!ShopWidgetClass)
	{
		return;
	}

	if (ShopWidgetInstance && IsValid(ShopWidgetInstance) && ShopWidgetInstance->IsInViewport())
	{
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	if (!IsValid(ShopWidgetInstance))
	{
		ShopWidgetInstance = CreateWidget<ULobbyShopUserWidget>(PC, ShopWidgetClass);
	}

	if (ShopWidgetInstance && !ShopWidgetInstance->IsInViewport())
	{
		ShopWidgetInstance->RefreshShopItems();
		ShopWidgetInstance->AddToViewport(200);
	}
}
