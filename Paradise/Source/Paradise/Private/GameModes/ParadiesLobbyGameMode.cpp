// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ParadiesLobbyGameMode.h"
#include "Controllers/LobbyPlayerController.h"

AParadiesLobbyGameMode::AParadiesLobbyGameMode()
{
	// MainMenu -> Lobby 시 PC가 유지되면 BeginPlay가 다시 안 돌아 로비 위젯/입력 모드가 갱신되지 않음.
	bUseSeamlessTravel = false;
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
}
