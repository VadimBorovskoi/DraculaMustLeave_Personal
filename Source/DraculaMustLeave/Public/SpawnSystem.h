// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnPoint.h"
#include "LevelEncounterDataAsset.h"
#include "SpawnSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API USpawnSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnSystem();
protected:
	TArray<ASpawnPoint*> SpawnPoints;
	ULevelEncounterDataAsset* CurrentEncounterDataAsset;
public:
	void SpawnAllObjects();
	void SpawnSpecificObject(TSubclassOf<ASpawnable> const SpawnableClass, float SpawnDelay);
	TArray<ASpawnPoint*> GetSpawnPointsWithTheClass(TSubclassOf<ASpawnable> const SpawnableClass);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};
