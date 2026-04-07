// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ShopItemDataRow.h"
#include "LobbyShopUserWidget.generated.h"

/**
 * 로비 상점 UI 기초. WBP에서 부모로 두고 레이아웃·상품은 블루프린트에서 확장.
 */
UCLASS()
class PARADISE_API ULobbyShopUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void RefreshShopItems();

protected:
	virtual void NativeConstruct() override;

private:
	void PopulateShopItems();

	UFUNCTION()
	void OnCloseClicked();

	UFUNCTION()
	void OnBackToLobbyClicked();

	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* CloseShopButton;

	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* BackToLobbyButton;

	/** 아이템 엔트리가 추가될 패널(예: VerticalBox, ScrollBox). */
	UPROPERTY(meta = (BindWidgetOptional))
	class UPanelWidget* ItemListPanel;

	/** Shop 데이터 테이블(DT). */
	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Shop|Data")
	class UDataTable* ShopItemsDataTable;

	/** 아이템 1개를 표시할 엔트리 위젯 클래스. */
	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Shop|Data")
	TSubclassOf<class ULobbyShopItemEntryWidget> ShopItemEntryWidgetClass;
};
