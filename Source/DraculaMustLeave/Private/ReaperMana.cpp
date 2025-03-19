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

	//Reduce Ghost Mana only if I should, check if the Ghost Mana cannot keep dropping anymore -> Start regenerating
	if (bShouldGhostManaDrop && TryReduceGhostMana(DeltaTime) == false)
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
			GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Blue, FString::Printf(TEXT("Mana Regeneration Cooldown: %f"), CurRegenCooldownTime));

		}
		//Regenerate Mana only if I should, check if Mana cannot keep regenerating anymore
		if (bShouldManaRegen && TryRegenerateMana(DeltaTime) == false)
		{
			bShouldManaRegen = false;
		}
		return;
	}
	//Only if Ghost Mana is bigger than Current Mana, do the Cooldown for Ghost Mana
	if (CurrentGhostMana > CurrentMana)
	{
		if (CurGhostManaCooldownTime > GhostManaCooldownTime)
		{
			bShouldGhostManaDrop = true;
			CurGhostManaCooldownTime = 0.f;
		} else
		{
			CurGhostManaCooldownTime += DeltaTime;
			GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Cyan, FString::Printf(TEXT("Ghost Mana Cooldown: %f"), CurRegenCooldownTime));
		}
	}
	
}
void UReaperMana::ReduceMana(float DeductedAmount, bool bShouldDropGhostManaAlong)
{
	CurrentMana = FMath::Max(CurrentMana - DeductedAmount, 0);
	if (bShouldDropGhostManaAlong)
	{
		CurrentGhostMana = FMath::Max(CurrentGhostMana - DeductedAmount, CurrentMana);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Mana Reduced Manually")));
	}
}
void UReaperMana::RestoreMana(float AddedAmount)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Mana Restored Manually")));
	CurrentMana = FMath::Min(CurrentMana + AddedAmount, CurrentGhostMana);
	//If Ghost Mana is Lower that Current Mana, equalize them
	//CurrentGhostMana = FMath::Max(CurrentGhostMana, CurrentMana);
}
bool UReaperMana::TryRegenerateMana(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("Trying To Regenerate Mana")));
	if (CurrentMana >= MaxMana)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Mana is Regenerated Fully")));
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
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("Trying To Reduce Ghost Mana")));
	if (CurrentGhostMana <= CurrentMana)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ghost Mana is Reduced Fully")));
		CurrentGhostMana = CurrentMana;
		bShouldGhostManaDrop = false;
		return false;
	}
	CurrentGhostMana -= GhostManaMaxVelocity * DeltaTime;
	return true;
}

bool UReaperMana::CanUseAbility(float AbilityMana)
{
	return AbilityMana <= CurrentMana;
}
