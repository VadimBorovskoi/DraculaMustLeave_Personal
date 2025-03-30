// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Spawnable.generated.h"
class ASpawnPoint;
class USpawnSystem;
UCLASS(Abstract, Blueprintable, ClassGroup=(Custom))
class DRACULAMUSTLEAVE_API ASpawnable : public AActor
{
	GENERATED_BODY()
protected:
	float SpawnDelay;
	USpawnSystem* EnemySpawnSystem;
	ASpawnPoint* AttachedSpawnPoint;
public:
	virtual void InitializeAfterSpawn(ASpawnPoint* SpawnPoint, USpawnSystem* SpawnSystem, float inSpawnDelay);
};