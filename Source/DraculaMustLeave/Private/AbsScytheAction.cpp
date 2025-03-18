// Fill out your copyright notice in the Description page of Project Settings.


#include "AbsScytheAction.h"

#include "Misc/AssetRegistryInterface.h"

// Sets default values for this component's properties
UAbsScytheAction::UAbsScytheAction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UAbsScytheAction::BeginPlay()
{
	Super::BeginPlay();
	Scythe = Cast<AScythe>(GetOwner());
	DefaultActionParameters = ActionParameters;
	ResetParameters();
	
	OnActivate.AddUniqueDynamic(this, &UAbsScytheAction::Enable);
	OnDeactivate.AddUniqueDynamic(this, &UAbsScytheAction::Disable);
	OnCharge.AddUniqueDynamic(this, &UAbsScytheAction::Charge);
	OnUpdate.AddUniqueDynamic(this, &UAbsScytheAction::Update);
	OnColliderOverlap.AddUniqueDynamic(this, &UAbsScytheAction::HandleColliderOverlap);
	OnMeshOverlap.AddUniqueDynamic(this, &UAbsScytheAction::HandleMeshOverlap);

	//Add Debug
	OnUpdate.AddDynamic(this, &UAbsScytheAction::DebugDrawOnTick);
	// ...
}
void UAbsScytheAction::UpdateParameters(FScytheActionParameters NewParams, bool bIsAdditive)
{
	if (bIsAdditive)
	{
		ParametersPool.Add(NewParams);
		return;
	}
	ActionParameters.SetNonZeroParams(NewParams);
}
void UAbsScytheAction::DebugDrawOnTick(float DeltaTime)
{
	if (Scythe->ScytheState == EScytheState::HELD || Scythe->ScytheState == EScytheState::STATIC) return;


	FString VelocityDebug = FString::Printf(TEXT("Max Velocity: %f"), ActionParameters.MaxVelocity);
	FString SpinDebug = FString::Printf(TEXT("Spin Rate: %f"), ActionParameters.RotationRate);
	FString DamageDebug = FString::Printf(TEXT("Damage: %f"), ActionParameters.DamagePerHit);
	FString ManaDebug = FString::Printf(TEXT("Damage: %f"), ActionParameters.ManaConsumption);
	
	DrawDebugString(GetWorld(), Scythe->GetActorLocation()  + FVector(0,0, 180.f), VelocityDebug, Scythe, FColor::White, 0.0f, false, 1.f);
	DrawDebugString(GetWorld(), Scythe->GetActorLocation() + FVector(0,0, 120.f), SpinDebug, Scythe, FColor::White, 0.0f, false, 1.f); 
	DrawDebugString(GetWorld(), Scythe->GetActorLocation() + FVector(0,0, 60.f), DamageDebug, Scythe, FColor::Red, 0.0f, false, 1.f); 
	DrawDebugString(GetWorld(), Scythe->GetActorLocation(), ManaDebug, Scythe, FColor::Blue, 0.0f, false, 1.f); 

}


