// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawnable.h"
#include "SpawnSystem.h"
void ASpawnable::InitializeAfterSpawn(ASpawnPoint* SpawnPoint, USpawnSystem* SpawnSystem, float inSpawnDelay)
{
	EnemySpawnSystem = SpawnSystem;
	AttachedSpawnPoint = SpawnPoint;
	SpawnDelay = inSpawnDelay;
}

