// Fill out your copyright notice in the Description page of Project Settings.


#include "ReaperMana.h"

// Sets default values for this component's properties
UReaperMana::UReaperMana()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UReaperMana::BeginPlay()
{
	Super::BeginPlay();
	CurrentMana = MaxMana;
	CurrentGhostMana = MaxMana;
	// ...
	
}
void UReaperMana::TickUpdate(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Reaper Mana Left %f"), CurrentMana);
	UE_LOG(LogTemp, Display, TEXT("Ghost Mana Left %f"), CurrentGhostMana);
	
	bool IsDropCountownInProcess = false;
	//Only if Ghost Mana is bigger than Current Mana, do the Cooldown for Ghost Mana
	if (CurrentGhostMana > CurrentMana)
	{
		if (CurGhostManaCooldownTime > GhostManaCooldownTime)
		{
			bShouldGhostManaDrop = true;
			CurGhostManaCooldownTime = 0.f;
		} else
		{
			IsDropCountownInProcess = true;
			CurGhostManaCooldownTime += DeltaTime;
		}
	}
	
	//Reduce Ghost Mana only if I should, check if the Ghost Mana cannot keep dropping anymore -> Start regenerating
	if ((bShouldGhostManaDrop && TryReduceGhostMana(DeltaTime) == false) || (!bShouldGhostManaDrop && !IsDropCountownInProcess))
	{
		//If the Cooldown is over, set Regeneration process
		if (CurRegenCooldownTime > ManaRegenCooldownTime)
		{
			bShouldManaRegen = true;
			CurRegenCooldownTime = 0.f;
		}
		//else, Update the Cooldown Timer
		else if (bShouldManaRegen == false)
		{
			CurRegenCooldownTime += DeltaTime;
		}
		//Regenerate Mana only if I should, check if Mana cannot keep regenerating anymore
		if (bShouldManaRegen && TryRegenerateMana(DeltaTime) == false)
		{
			bShouldManaRegen = false;
		}
	}
}

void UReaperMana::ReduceMana(float DeductedAmount, bool bShouldDropGhostManaAlong)
{
	bool GhostEqualsMana = CurrentGhostMana <= CurrentMana;
	CurrentMana = FMath::Max(CurrentMana - DeductedAmount, 0);
	bShouldManaRegen = false;
	if (bShouldDropGhostManaAlong)
	{
		bShouldGhostManaDrop = false;
		if (GhostEqualsMana == false) CurrentGhostMana = FMath::Max(CurrentGhostMana - DeductedAmount, CurrentMana);
	}
}
void UReaperMana::RestoreMana(float AddedAmount)
{
	CurrentMana = FMath::Min(CurrentMana + AddedAmount, CurrentGhostMana);
	//If Ghost Mana is Lower that Current Mana, equalize them
	//CurrentGhostMana = FMath::Max(CurrentGhostMana, CurrentMana);
}
bool UReaperMana::TryRegenerateMana(float DeltaTime)
{
	if (CurrentMana >= MaxMana)
	{
		CurrentMana = MaxMana;
		CurrentGhostMana = CurrentMana;
		return false;
	}
	CurrentMana += ManaRegenMaxVelocity * DeltaTime;
	CurrentGhostMana = CurrentMana;
	return true;
}
bool UReaperMana::TryReduceGhostMana(float DeltaTime)
{
	if (CurrentGhostMana <= CurrentMana)
	{
		CurrentGhostMana = CurrentMana;
		bShouldGhostManaDrop = false;
		return false;
	}
	CurrentGhostMana -= GhostManaMaxVelocity * DeltaTime;
	return true;
}

bool UReaperMana::CanUseAbility(float AbilityMana)
{
	//return AbilityMana <= CurrentMana;
	return CurrentMana > 0.0f;
}
