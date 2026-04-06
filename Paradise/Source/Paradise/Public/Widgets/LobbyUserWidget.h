// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/LobbyShopUserWidget.h"
#include "LobbyUserWidget.generated.h"

/**
 * 로비: 게임방 생성·참가, 상점 열기.
 * 필수: CreateGameButton, JoinGameButton. 선택: OpenShopButton, JoinAddressField.
 */
UCLASS()
class PARADISE_API ULobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnCreateGameClicked();

	UFUNCTION()
	void OnJoinGameClicked();

	UFUNCTION()
	void OnOpenShopClicked();

	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Travel")
	FName GameplayLevelName = FName(TEXT("GameMap"));

	/** JoinAddressField이 비었을 때 사용. 예: 127.0.0.1:7777 */
	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Travel")
	FString DefaultJoinAddress = TEXT("127.0.0.1:7777");

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidgetOptional))
	class UEditableTextBox* JoinAddressField;

	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* OpenShopButton;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Shop")
	TSubclassOf<ULobbyShopUserWidget> ShopWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<ULobbyShopUserWidget> ShopWidgetInstance;
};
