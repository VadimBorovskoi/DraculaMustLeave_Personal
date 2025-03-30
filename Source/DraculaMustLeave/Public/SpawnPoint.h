// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawnable.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "SpawnPoint.generated.h"

UCLASS()
class DRACULAMUSTLEAVE_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* BillboardComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	TArray<TSubclassOf<ASpawnable>> SpawnablesAllowed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	FVector2D RangeXY = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	int MaxObjectsAtOnce = 1;
	
protected:
	int CurrentSpawnObjects = 0;

public:
	bool TrySpawnObject(TSubclassOf<ASpawnable> const SpawnableClass, float SpawnDelay, USpawnSystem* SpawnSystem);
	bool IsFullyOccupied() {return CurrentSpawnObjects >= MaxObjectsAtOnce;}
	void UnregisterSpawnedObject();
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif	
protected:
	bool CanSpawnObject(TSubclassOf<ASpawnable> const SpawnableClass);
	UFUNCTION()
	void SpawnObject(TSubclassOf<ASpawnable> const SpawnableClass, float const inSpawnDelay, USpawnSystem* SpawnSystem);
	void DrawSpawnRange();

};
