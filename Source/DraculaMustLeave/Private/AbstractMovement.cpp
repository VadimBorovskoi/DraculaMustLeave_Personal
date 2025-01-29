// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractMovement.h"

// Sets default values for this component's properties
UAbstractMovement::UAbstractMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}
bool UAbstractMovement::IsDashBlocked(FVector PotentialPosition, float Radius, float HalfHeight, ECollisionChannel CollisionChannel)
{
	TArray<FHitResult> OutHits;
	TArray<AActor*> IgnoredActors;
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	return UKismetSystemLibrary::CapsuleTraceMulti(
		this, PotentialPosition, PotentialPosition, Radius, HalfHeight, UEngineTypes::ConvertToTraceType(CollisionChannel),
		false, IgnoredActors, DebugType, OutHits, true
	);
}





