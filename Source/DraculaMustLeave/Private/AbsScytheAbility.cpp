// Fill out your copyright notice in the Description page of Project Settings.


#include "AbsScytheAbility.h"
#include "AbsScytheAction.h"
#include "TypeUtil.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAbsScytheAbility::UAbsScytheAbility()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UAbsScytheAbility::BeginPlay()
{
	Super::BeginPlay();
	
	OnActivate.AddUniqueDynamic(this, &UAbsScytheAbility::Enable);
	OnDeactivate.AddUniqueDynamic(this, &UAbsScytheAbility::Disable);
	OnUpdate.AddUniqueDynamic(this, &UAbsScytheAbility::Update);
	OnCharge.AddUniqueDynamic(this, &UAbsScytheAbility::Charge);
	OnMeshOverlap.AddUniqueDynamic(this, &UAbsScytheAbility::HitMesh);
	OnColliderOverlap.AddUniqueDynamic(this, &UAbsScytheAbility::HitCollision);

	TArray<AActor*> FoundActors;
	Activate(Cast<AScythe>(GetOwner()));
	
}
void UAbsScytheAbility::Activate(AScythe* NewScythe)
{
	Scythe = NewScythe;
	ConnectedAction = Cast<UAbsScytheAction>(Scythe->GetComponentByClass(ConnectedActionClass));
	ConnectedAction->AbilityArray.AddUnique(this);
	if (ActivationParameters.bShouldCharge) ConnectedAction->OnCharge.AddUniqueDynamic(this, &UAbsScytheAbility::HandleCharge);
}

void UAbsScytheAbility::MergeDelegates()
{
	
}

bool UAbsScytheAbility::IsEnabled()
{
	bool bIsPossibleToEnable = true;
	if (ActivationParameters.bShouldHaveActivationWindow)
	{
		bIsPossibleToEnable = ActivationParameters.bCanActivationWindowOpen && ActivationParameters.bCanActivateWithinWindow;
	}
	if (ActivationParameters.bShouldCharge)
	{
		bIsPossibleToEnable = ActivationParameters.bIsCharged;
	}
	return bIsPossibleToEnable;
}




