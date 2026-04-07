#include "Widgets/LobbyShopItemEntryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULobbyShopItemEntryWidget::SetItemData(const FShopItemDataRow& InItemData)
{
	if (ItemNameText)
	{
		ItemNameText->SetText(InItemData.ItemName);
	}

	if (PriceText)
	{
		PriceText->SetText(FText::AsNumber(InItemData.Price));
	}

	if (DescriptionText)
	{
		DescriptionText->SetText(InItemData.Description);
	}

	if (IconImage)
	{
		UTexture2D* IconTexture = InItemData.Icon.Get();
		if (!IconTexture && !InItemData.Icon.IsNull())
		{
			IconTexture = InItemData.Icon.LoadSynchronous();
		}
		IconImage->SetBrushFromTexture(IconTexture, true);
	}
}
