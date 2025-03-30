// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "SpawnSystem.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
}
void AEnemyBase::InitializeAfterSpawn(ASpawnPoint* SpawnPoint, USpawnSystem* SpawnSystem, float inSpawnDelay)
{
	Super::InitializeAfterSpawn(SpawnPoint, SpawnSystem, inSpawnDelay);
	Health = GetComponentByClass<UHealthBase>();
	if (!Health) Health = CreateDefaultSubobject<UHealthBase>(TEXT("Arrow"));
	Health->OnDie.AddUniqueDynamic(this, &AEnemyBase::HandleRespawn);
}
void AEnemyBase::HandleRespawn(AActor* Killer, UObject* KillingTool)
{
	Health->OnDie.RemoveDynamic(this, &AEnemyBase::HandleRespawn);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy Dead, beginning respawn")));
	AttachedSpawnPoint->UnregisterSpawnedObject();
	EnemySpawnSystem->SpawnSpecificObject(GetClass(), SpawnDelay);
}



