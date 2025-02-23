// Fill out your copyright notice in the Description page of Project Settings.


#include "ScythePierce.h"

void UScythePierce::MergeDelegates()
{
	Super::MergeDelegates();
	SecondaryAction->OnUpdate.AddUniqueDynamic(this, &UScythePierce::DetectActivationWindow);
}
void UScythePierce::Activate(AScythe* NewScythe)
{
	Super::Activate(NewScythe);
}
void UScythePierce::Enable(float xDir, FVector TargetPoint)
{
	
}
void UScythePierce::Disable()
{
	
}
void UScythePierce::Charge(float ElapsedTime)
{
	
}
void UScythePierce::HitMesh(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
void UScythePierce::HitCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
void UScythePierce::DetectActivationWindow(float DeltaSeconds)
{
	
}
void UScythePierce::CheckCorrectTiming()
{
	if (ActivationParameters.bCanActivateWithinWindow)
	{
		//Update the parameters of the Scythe Throw Action
	} else
	{
		ActivationParameters.bCanActivationWindowOpen = false;
	}
}

