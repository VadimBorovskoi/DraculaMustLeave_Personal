// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Spawnable.h"
#include "LevelEncounterDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEncounterData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ASpawnable> ClassToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int ObjectsNumberToSpawn = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float SpawnDelay = 0.5f;
};
UCLASS()
class DRACULAMUSTLEAVE_API ULevelEncounterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FEncounterData> EncounterDataStructs;
	static ULevelEncounterDataAsset* GetCurrentLevelAsset(UObject* WorldContextObject);
};
