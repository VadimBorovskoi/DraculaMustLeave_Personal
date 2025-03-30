// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

APlayerHUD::APlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	AReaperGameState* GameState = Cast<AReaperGameState>(GetWorld()->GetGameState());
	if (!GameState) return;
	GameState->OnPaused.AddUniqueDynamic(this, &APlayerHUD::DisplayPauseScreen);
	GameState->OnPlaying.AddUniqueDynamic(this, &APlayerHUD::DisplayPlayerHUD);
	GameState->OnGameOver.AddUniqueDynamic(this, &APlayerHUD::DisplayGameOver);
}
