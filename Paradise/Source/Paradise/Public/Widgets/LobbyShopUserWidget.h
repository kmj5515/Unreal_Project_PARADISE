// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyShopUserWidget.generated.h"

/**
 * 로비 상점 UI 기초. WBP에서 부모로 두고 레이아웃·상품은 블루프린트에서 확장.
 */
UCLASS()
class PARADISE_API ULobbyShopUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnCloseClicked();

	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* CloseShopButton;
};
