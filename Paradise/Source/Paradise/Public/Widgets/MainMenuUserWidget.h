// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PARADISE_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* MainSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* characterSelectionRoot;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartCharacterSelectionButton;
};
