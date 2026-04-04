// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.generated.h"

/**
 * 메인 메뉴: 로비로 이동(?listen), 종료. 캐릭터 선택은 로비에서 처리.
 * 로비 맵 에셋 이름은 에디터에서 LobbyLevelName에 맞게 설정하세요.
 */
UCLASS()
class PARADISE_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnStartGameClicked();

	UFUNCTION()
	void OnQuitGameClicked();

	UPROPERTY(EditDefaultsOnly, Category = "Menu|Travel")
	FName LobbyLevelName = FName(TEXT("LobbyMap"));

	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;
};
