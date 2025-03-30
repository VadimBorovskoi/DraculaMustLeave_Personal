// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ReaperGameMode.h"
#include "ReaperGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerChange, int, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameModeStateChanged, int, CurrentScore);

UCLASS()
class DRACULAMUSTLEAVE_API AReaperGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AReaperGameState();
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnTimerChange OnTimerChange;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnGameModeStateChanged OnPlaying;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnGameModeStateChanged OnPaused;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnGameModeStateChanged OnGameOver;

	UPROPERTY(BlueprintReadWrite, Category="Time")
	int32 CurrentTime;
protected:
	AReaperGameMode* ReaperGameMode;
	EGameModeState GameModeState = EGameModeState::PLAYING;
	TMap<EGameModeState, FOnGameModeStateChanged*> GameModeStatesMap;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void DebugGameOver(int CurrentScore);
public:
	UFUNCTION(BlueprintCallable, Category="State Management")
 	void ReduceTime(int32 const TimeDeducted);
	UFUNCTION(BlueprintCallable, Category="State Management")
	void SetNewGameModeState(EGameModeState const NewGameModeState, int32 UpdatedScore);
	UFUNCTION(BlueprintCallable, Category="State Management")
	void TogglePause(int UpdatedScore);
};
