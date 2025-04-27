// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsPushable.h"

// Sets default values for this component's properties
UPhysicsPushable::UPhysicsPushable()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPhysicsPushable::BeginPlay()
{
	Super::BeginPlay();
	RootComponent = GetOwner()->GetComponentByClass<UPrimitiveComponent>();
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); // Ignore self
	FVector Start = RootComponent->GetComponentLocation();
	FVector End = Start + (FVector::DownVector * 10000.f);
	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, SurfaceDetectionChannel, QueryParams);
	InitialCachedHeight = HitResult.Distance;
}
void UPhysicsPushable::UpdateOwner(float DeltaTime)
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); // Ignore self
	FVector Start = RootComponent->GetComponentLocation();
	FVector End = Start + (FVector::DownVector * 10000.f);
	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,SurfaceDetectionChannel, QueryParams);
	
	if (RootComponent->GetComponentVelocity().Z < .0f && FMath::IsNearlyEqual(HitResult.Distance, InitialCachedHeight, 10.f) )
	{
		RootComponent->SetEnableGravity(false);
		RootComponent->SetSimulatePhysics(false);
	}
	if (bShouldResetPhysics && FMath::IsNearlyEqual(HitResult.Distance, InitialCachedHeight, 10.f) )
	{
		RootComponent->SetEnableGravity(false);
		RootComponent->SetSimulatePhysics(false);
		RootComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
		FVector NewPos = GetOwner()->GetActorLocation();
		NewPos.Z = HitResult.ImpactPoint.Z + InitialCachedHeight;
		GetOwner()->SetActorLocation(NewPos);
	}
	if (RootComponent->GetComponentVelocity().Z > .0f)
	{
		if (CurrentGravityDelay <= .0f) RootComponent->SetEnableGravity(true);
		else CurrentGravityDelay -= DeltaTime;
	}
}
void UPhysicsPushable::ApplyForce(const FVector ImpactPosition, const FVector Direction, const float Force)
{
	RootComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	RootComponent->SetSimulatePhysics(true);
	RootComponent->AddImpulse(Direction.GetSafeNormal() * Force);
	OnPush.Broadcast(ImpactPosition, Direction, Force);
	bShouldResetPhysics = false;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Object Launched")));
	CurrentGravityDelay = GravityDelay;
}
void UPhysicsPushable::ResetForce()
{
	bShouldResetPhysics = true;
}

void UPhysicsPushable::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UpdateOwner(DeltaTime);
}

