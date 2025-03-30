// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReaperPawn.h"
#include "ReaperGameState.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class DRACULAMUSTLEAVE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	APlayerHUD();
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void DisplayGameOver(int FinalScore);
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void DisplayPauseScreen(int CurrentScore);
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void DisplayPlayerHUD(int CurrentScore);
protected:
	virtual void BeginPlay() override;
};
