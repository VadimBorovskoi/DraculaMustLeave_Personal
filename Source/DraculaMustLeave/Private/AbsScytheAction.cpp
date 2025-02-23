// Fill out your copyright notice in the Description page of Project Settings.


#include "AbsScytheAction.h"

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
	
	OnActivate.AddUniqueDynamic(this, &UAbsScytheAction::Enable);
	OnDeactivate.AddUniqueDynamic(this, &UAbsScytheAction::Disable);
	OnCharge.AddUniqueDynamic(this, &UAbsScytheAction::Charge);
	OnUpdate.AddUniqueDynamic(this, &UAbsScytheAction::Update);
	OnColliderOverlap.AddUniqueDynamic(this, &UAbsScytheAction::HandleColliderOverlap);
	OnMeshOverlap.AddUniqueDynamic(this, &UAbsScytheAction::HandleMeshOverlap);
	// ...
}


