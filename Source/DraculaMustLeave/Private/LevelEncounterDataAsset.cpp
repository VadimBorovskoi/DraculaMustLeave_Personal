// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEncounterDataAsset.h"

#include "Kismet/GameplayStatics.h"

ULevelEncounterDataAsset* ULevelEncounterDataAsset::GetCurrentLevelAsset(UObject* WorldContextObject)
{
	
	FString LevelName = UGameplayStatics::GetCurrentLevelName(WorldContextObject, true);
	FSoftObjectPath AssetPath(FString::Printf(TEXT("/Game/Blueprints/SpawnSystem/Data/DA_%s.DA_%s"), *LevelName, *LevelName));
	ULevelEncounterDataAsset* EncounterDataAsset = Cast<ULevelEncounterDataAsset>(AssetPath.TryLoad());

	return EncounterDataAsset;
}
