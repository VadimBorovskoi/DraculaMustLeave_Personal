// Fill out your copyright notice in the Description page of Project Settings.


#include "ReaperGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ReaperPawn.h"

AReaperGameState::AReaperGameState(){
	PrimaryActorTick.bCanEverTick = false;
	GameModeStatesMap.Add(EGameModeState::PLAYING, &OnPlaying);
	GameModeStatesMap.Add(EGameModeState::PAUSED, &OnPaused);
	GameModeStatesMap.Add(EGameModeState::GAME_OVER, &OnGameOver);
	OnGameOver.AddUniqueDynamic(this, &AReaperGameState::DebugGameOver);
}
void AReaperGameState::BeginPlay(){
	Super::BeginPlay();
	ReaperGameMode = Cast<AReaperGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!ReaperGameMode) return;
	CurrentTime = ReaperGameMode->TimeToPlay;

}
void AReaperGameState::ReduceTime(int32 const TimeDeducted){
	if(!ReaperGameMode) return;

	CurrentTime = FMath::Max(CurrentTime - TimeDeducted, 0);

	OnTimerChange.Broadcast(CurrentTime);

	if(ReaperGameMode->TimeToPlay > 0 && CurrentTime <= 0){
		AReaperPawn* Reaper = Cast<AReaperPawn>( GetWorld()->GetFirstPlayerController()->GetPawn());
		if(Reaper){
			SetNewGameModeState(EGameModeState::GAME_OVER, Reaper->ComboHealth->GetCurrentStage());
		}
	}
}
void AReaperGameState::SetNewGameModeState(EGameModeState const NewGameModeState, int32 UpdatedScore){
	GameModeState = NewGameModeState;
	GameModeStatesMap[NewGameModeState]->Broadcast(UpdatedScore);
}
void AReaperGameState::DebugGameOver(int Score)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("GAME OVER")));
}
void AReaperGameState::TogglePause(int UpdatedScore){
	if(GameModeState == EGameModeState::PLAYING){
		SetNewGameModeState(EGameModeState::PAUSED, UpdatedScore);
		return;
	}
	if(GameModeState == EGameModeState::PAUSED){
		SetNewGameModeState(EGameModeState::PLAYING, UpdatedScore);
		return;
	}
}