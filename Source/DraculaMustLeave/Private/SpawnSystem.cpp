// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/RandomShuffle.h"

// Sets default values for this component's properties
USpawnSystem::USpawnSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USpawnSystem::BeginPlay()
{
	Super::BeginPlay();
	// ...
	CurrentEncounterDataAsset = ULevelEncounterDataAsset::GetCurrentLevelAsset(this);
	if (!CurrentEncounterDataAsset)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Data Asset Not Loaded!")));
		return;
	} 

	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ASpawnPoint::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(Actor);
		if (SpawnPoint)
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}
	SpawnAllObjects();
}
void USpawnSystem::SpawnAllObjects()
{
	for (int i = 0; i < CurrentEncounterDataAsset->EncounterDataStructs.Num(); i++)
	{
		TSubclassOf<ASpawnable> ActorClassToSpawn = CurrentEncounterDataAsset->EncounterDataStructs[i].ClassToSpawn;
		if (!ActorClassToSpawn) continue;

		float SpawnDelay = CurrentEncounterDataAsset->EncounterDataStructs[i].SpawnDelay;
		TArray<ASpawnPoint*> FilteredSpawnPoints = GetSpawnPointsWithTheClass(CurrentEncounterDataAsset->EncounterDataStructs[i].ClassToSpawn);
		int AssetClassIndex = 0;
		int SpawnPointIndex = 0;
		while (FilteredSpawnPoints.IsEmpty() == false
			&& AssetClassIndex < CurrentEncounterDataAsset->EncounterDataStructs[i].ObjectsNumberToSpawn)
		{
			if (SpawnPointIndex == 0) Algo::RandomShuffle(FilteredSpawnPoints);
			if (FilteredSpawnPoints[SpawnPointIndex]->TrySpawnObject(ActorClassToSpawn, SpawnDelay, this))
			{
				AssetClassIndex++;
				SpawnPointIndex++;
				if (SpawnPointIndex >= FilteredSpawnPoints.Num()) SpawnPointIndex = 0;
			} else
			{
				FilteredSpawnPoints.RemoveAt(SpawnPointIndex);
			}
		}
	}
}
void USpawnSystem::SpawnSpecificObject(TSubclassOf<ASpawnable> const SpawnableClass, float SpawnDelay)
{
	if (SpawnPoints.IsEmpty()) 
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(this, ASpawnPoint::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(Actor);
			if (SpawnPoint)
			{
				SpawnPoints.Add(SpawnPoint);
			}
		}
	}
	if (SpawnPoints.IsEmpty() == false) Algo::RandomShuffle(SpawnPoints);
	for (int i = 0; i < SpawnPoints.Num(); i++)
	{
		if (SpawnPoints[i]->TrySpawnObject(SpawnableClass, SpawnDelay, this)) break;
	}
}

TArray<ASpawnPoint*> USpawnSystem::GetSpawnPointsWithTheClass(TSubclassOf<ASpawnable> const SpawnableClass)
{
	TArray<ASpawnPoint*> FilteredSpawnPoints;

	for (ASpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (!SpawnPoint) continue;

		// Check if SpawnPoint can spawn the required class
		if (SpawnPoint->SpawnablesAllowed.Contains(SpawnableClass))
		{
			FilteredSpawnPoints.Add(SpawnPoint);
		}
	}
	
	return FilteredSpawnPoints;
}


