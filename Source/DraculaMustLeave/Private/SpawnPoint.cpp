// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "TypeUtil.h"
#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	BillboardComponent->SetupAttachment(RootComponent);
#if WITH_EDITOR
	DrawSpawnRange();
#endif
}

// Called when the game starts or when spawned
bool ASpawnPoint::TrySpawnObject(TSubclassOf<ASpawnable> const SpawnableClass, float SpawnDelay, USpawnSystem* SpawnSystem)
{
	bool bCanSpawn = CanSpawnObject(SpawnableClass);
	if (!bCanSpawn) return false;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this, SpawnSystem, SpawnableClass, SpawnDelay]() 
	{
		SpawnObject(SpawnableClass, SpawnDelay, SpawnSystem);
	});
	FTimerHandle TimerHandle;
	CurrentSpawnObjects++;
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, SpawnDelay, false);
	return true;
}

// Called every frame
bool ASpawnPoint::CanSpawnObject(TSubclassOf<ASpawnable> const SpawnableClass)
{
	return SpawnableClass != nullptr && IsFullyOccupied() == false
	&& (SpawnablesAllowed.IsEmpty() || SpawnablesAllowed.Contains(SpawnableClass));
}
void ASpawnPoint::SpawnObject(TSubclassOf<ASpawnable> const SpawnableClass, float const inSpawnDelay, USpawnSystem* SpawnSystem)
{
	FVector SpawnPoint = GetActorLocation();
	if (RangeXY.IsNearlyZero() == false)
	{
		FVector2D RandomisedPos = UTypeUtil::GetRandomVector2DInRange( FVector2D(SpawnPoint.X - RangeXY.X, SpawnPoint.Y - RangeXY.Y),
		FVector2D(SpawnPoint.X + RangeXY.X, SpawnPoint.Y + RangeXY.Y));
		SpawnPoint = FVector(RandomisedPos.X, RandomisedPos.Y, SpawnPoint.Z);
	}
	ASpawnable* SpawnedInstance =  GetWorld()->SpawnActor<ASpawnable>(SpawnableClass, SpawnPoint, FRotator::ZeroRotator);
	if (SpawnedInstance) SpawnedInstance->InitializeAfterSpawn(this, SpawnSystem, inSpawnDelay);
}
void ASpawnPoint::UnregisterSpawnedObject()
{
	CurrentSpawnObjects = FMath::Max(0, CurrentSpawnObjects - 1);
}

#if WITH_EDITOR
void ASpawnPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	DrawSpawnRange();
}

void ASpawnPoint::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	DrawSpawnRange();
}

void ASpawnPoint::DrawSpawnRange()
{
	if (!RangeXY.IsNearlyZero() && GetWorld())
	{
		FVector SpawnPoint = GetActorLocation();
		const int NumSegments = 16;
		float AngleStep = 2 * PI / NumSegments;

		for (int i = 0; i < NumSegments; i++)
		{
			float Angle1 = i * AngleStep;
			float Angle2 = (i + 1) * AngleStep;

			FVector Point1 = SpawnPoint + FVector(FMath::Cos(Angle1) * RangeXY.X, FMath::Sin(Angle1) * RangeXY.Y, 0);
			FVector Point2 = SpawnPoint + FVector(FMath::Cos(Angle2) * RangeXY.X, FMath::Sin(Angle2) * RangeXY.Y, 0);

			DrawDebugLine(GetWorld(), Point1, Point2, FColor::White, false, 2.f, 0, 2.0f); // ✅ Persistent (-1.0f duration)
		}
	}
}

#endif
