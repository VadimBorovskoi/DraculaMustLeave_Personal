// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReaperGameMode.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EGameModeState
{
	PLAYING,     
	PAUSED, 
	GAME_OVER
};

UCLASS()
class DRACULAMUSTLEAVE_API AReaperGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int TimeToPlay = 120;
};
