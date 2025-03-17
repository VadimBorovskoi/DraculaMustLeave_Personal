// Fill out your copyright notice in the Description page of Project Settings.


#include "ScythePierce.h"


void UScythePierce::AttachToAction(AScythe* NewScythe)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Combo Activated Within Window")));
	//Rewrite the struct of parameters
	ConnectedAction->UpdateParameters(OverridenActionParameters, true);
}
void UScythePierce::DetachFromAction(AScythe* NewScythe)
{
	//Reset the struct of parameters
	ConnectedAction->ResetParameters();
}


void UScythePierce::Activate(AScythe* NewScythe)
{
	Super::Activate(NewScythe);
	UE_LOG(LogTemp, Warning, TEXT("Pierce Attack Activated"));
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
void UScythePierce::Update(float DeltaSeconds)
{
	
}

void UScythePierce::HitMesh(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
void UScythePierce::HitCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}



