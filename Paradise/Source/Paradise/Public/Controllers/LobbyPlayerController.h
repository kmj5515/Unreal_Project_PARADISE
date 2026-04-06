// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/MenuPlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 로비 맵용 PC: 메뉴 위젯 유지 + 캐릭터 이동을 위해 GameAndUI 입력 모드.
 */
UCLASS()
class PARADISE_API ALobbyPlayerController : public AMenuPlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
