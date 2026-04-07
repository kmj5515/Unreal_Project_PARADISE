#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ShopItemDataRow.generated.h"

/**
 * 상점 아이템 데이터 행.
 */
USTRUCT(BlueprintType)
struct PARADISE_API FShopItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShopItem")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShopItem")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShopItem")
	int32 Price = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShopItem")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShopItem")
	FText Description;
};
