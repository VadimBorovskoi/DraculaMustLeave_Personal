// Fill out your copyright notice in the Description page of Project Settings.


#include "ScytheHand.h"
#include "Scythe.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values for this component's properties
UScytheHand::UScytheHand()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	ScytheMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ScytheMesh");
	ScytheMesh->SetupAttachment(this);
	ScytheMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// Called when the game starts
void UScytheHand::BeginPlay()
{
	Super::BeginPlay();
	Reaper = Cast<AReaperPawn>(GetOwner());
	HolsterScythe();
}

void UScytheHand::ScythePress()
{
	Scythe->ReceiveInput();
}
void UScytheHand::ScytheHold(float ElapsedSeconds)
{
	Scythe->Charge(ElapsedSeconds);
}
void UScytheHand::ScytheRelease()
{
	Scythe->Release();
}

void UScytheHand::HolsterScythe()
{
	if (!Scythe)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		UE_LOG(LogTemp, Log, TEXT("Component location: %s"), *GetComponentLocation().ToString());
		Scythe = GetWorld()->SpawnActor<AScythe>(ScytheType, GetComponentLocation(), GetComponentRotation(), SpawnParams);
		UE_LOG(LogTemp, Log, TEXT("Scythe location: %s"), *Scythe->GetActorLocation().ToString());

	}
	Scythe->SetOwnerHand(this);
	Scythe->Hide();
	Scythe->DisableCollision();
	SetLocalScytheVisibility(true);
}
void UScytheHand::SetLocalScytheVisibility(bool IsVisible) const
{
	ScytheMesh->SetVisibility(IsVisible);
}
FVector UScytheHand::GetCrosshairTarget() const
{
	return Reaper->Camera->GetForwardVector() * 100000.f;
}
FVector UScytheHand::GetReaperMovementDirection() const
{
	return Reaper->GetMovementComponent()->Velocity;
}


