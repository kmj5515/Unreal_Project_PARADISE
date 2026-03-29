// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ParadiseCharacterBase.h"
#include "ParadiseEnemyCharacter.generated.h"

/**
 * 적(좀비 등)용 GAS 캐릭터 베이스.
 * 청각/시야 탐지, 추격, 근접 공격은 보통 AIController + Behavior Tree + Blackboard + Perception으로 구성합니다.
 */
UCLASS(Blueprintable)
class PARADISE_API AParadiseEnemyCharacter : public AParadiseCharacterBase
{
	GENERATED_BODY()

public:
	AParadiseEnemyCharacter();
};
