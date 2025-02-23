// Fill out your copyright notice in the Description page of Project Settings.


#include "AbsScytheAbility.h"

#include "TypeUtil.h"

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
	
	
}
void UAbsScytheAbility::Activate(AScythe* NewScythe)
{
	Scythe = NewScythe;
	ConnectedAction = Cast<UAbsScytheAction>(Scythe->GetComponentByClass(ConnectedActionClass));
	ConnectedAction->OnActivate.AddUniqueDynamic(this, &UAbsScytheAbility::Enable);
	if (bIsAdditiveEffects)
	{
		MergeDelegates();
	} else
	{
		ConnectedAction->OnActivate = OnActivate;
        ConnectedAction->OnDeactivate = OnDeactivate;
		ConnectedAction->OnCharge = OnCharge;
		ConnectedAction->OnUpdate = OnUpdate;
		ConnectedAction->OnColliderOverlap = OnColliderOverlap;
		ConnectedAction->OnMeshOverlap = OnMeshOverlap;
	}
	if (bIsAdditiveParams)
	{
		//Add one struct to another (requires overriding the + of the struct
	} else
	{
		ConnectedAction->ActionParameters = OverridenActionParameters;
	}
}
void UAbsScytheAbility::MergeDelegates()
{
	ConnectedAction->OnActivate.AddUniqueDynamic(this, &UAbsScytheAbility::HandleActivate);
	ConnectedAction->OnDeactivate.AddUniqueDynamic(this, &UAbsScytheAbility::HandleDeactivate);
	ConnectedAction->OnCharge.AddUniqueDynamic(this, &UAbsScytheAbility::HandleCharge);
	ConnectedAction->OnUpdate.AddUniqueDynamic(this, &UAbsScytheAbility::HandleUpdate);
	ConnectedAction->OnMeshOverlap.AddUniqueDynamic(this, &UAbsScytheAbility::HandleMeshHit);
	ConnectedAction->OnColliderOverlap.AddUniqueDynamic(this, &UAbsScytheAbility::HandleCollisionHit);
}

bool UAbsScytheAbility::IsEnabled()
{
	bool bIsEnabled = true;
	if (ActivationParameters.bShouldHaveActivationWindow)
	{
		bIsEnabled = ActivationParameters.bCanActivationWindowOpen && ActivationParameters.bCanActivateWithinWindow;
	}
	if (ActivationParameters.bShouldCharge)
	{
		bIsEnabled = ActivationParameters.bIsCharged;
	}
	return bIsEnabled;
}




