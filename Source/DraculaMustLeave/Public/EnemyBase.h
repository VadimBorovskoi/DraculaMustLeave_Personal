// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBase.h"
#include "Spawnable.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UCLASS()
class DRACULAMUSTLEAVE_API AEnemyBase : public ASpawnable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorComponents")
	TObjectPtr<UHealthBase> Health;

public:
	virtual void InitializeAfterSpawn(ASpawnPoint* SpawnPoint, USpawnSystem* SpawnSystem, float inSpawnDelay) override;	
private:
	UFUNCTION()
	virtual void HandleRespawn(AActor* Killer, UObject* KillingTool);
};
