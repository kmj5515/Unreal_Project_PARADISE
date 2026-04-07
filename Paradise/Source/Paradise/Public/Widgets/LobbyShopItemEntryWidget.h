#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ShopItemDataRow.h"
#include "LobbyShopItemEntryWidget.generated.h"

/**
 * 상점 아이템 1개를 표시하는 엔트리 위젯.
 */
UCLASS()
class PARADISE_API ULobbyShopItemEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItemData(const FShopItemDataRow& InItemData);

private:
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* PriceText;

	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* IconImage;
};
