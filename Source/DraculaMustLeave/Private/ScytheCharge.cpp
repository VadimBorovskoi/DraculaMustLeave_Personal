// Fill out your copyright notice in the Description page of Project Settings.


#include "ScytheCharge.h"

#include "AbsScytheAction.h"

void UScytheCharge::AttachToAction(AScythe* NewScythe)
{
	//Rewrite the struct of parameters, Add new Update Behavior & On Hit Collision Behavior
	ConnectedAction->UpdateParameters(OverridenActionParameters, false);
}
void UScytheCharge::DetachFromAction(AScythe* NewScythe)
{
	//Reset the struct of parameters, Have the Action Reset its Behaviors
	ConnectedAction->ResetParameters();
}

void UScytheCharge::Activate(AScythe* NewScythe)
{
	Super::Activate(NewScythe);
}
void UScytheCharge::Enable(float xDir, FVector TargetPoint)
{
	
}
void UScytheCharge::Disable()
{
	
}
void UScytheCharge::Charge(float ElapsedTime)
{
	if (ElapsedTime >= ActivationParameters.RequiredChargeTime)
	{
		AttachToAction(Scythe);
		GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Green, FString::Printf(TEXT("Heavy Attack Charged")));
	}
}
void UScytheCharge::Update(float DeltaSeconds)
{
	
}

void UScytheCharge::HitMesh(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
void UScytheCharge::HitCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}