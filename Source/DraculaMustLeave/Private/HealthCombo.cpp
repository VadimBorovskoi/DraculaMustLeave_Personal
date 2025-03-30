// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCombo.h"
#include "InterpolationUtil.h"

UHealthCombo::UHealthCombo()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UHealthCombo::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = 1.f;
}

void UHealthCombo::ReceiveDamage(AActor* Sender, UObject* DamageSource, float& Damage, bool& ShouldStopScythe)
{
	//Reduce Health
	CurrentHealth = FMath::Min(0.f, CurrentHealth - Damage);
	//Slightly Increase Health for the Last Hit
	CurrentHealth = CurrentHealth < LastHitHealth ? LastHitHealth : CurrentHealth;
	UE_LOG(LogTemp, Warning, TEXT("Reaper Health Left %f"), CurrentHealth);
	//Cooldown for Regeneration
	CurrentCooldownTime = CooldownTimeSec;
	OnUpdateHealth.Broadcast(CurrentHealth);
	//Check for death
	if (CurrentHealth <= 0.f)
	{
		Die(Sender, DamageSource);
		return;
	}
	OnHurt.Broadcast(Sender, DamageSource);

	UpdateStage();

}
void UHealthCombo::Die(AActor* Sender, UObject* DamageSource)
{
	Super::Die(Sender, DamageSource);
}
void UHealthCombo::AddHealth(float DamagePoints)
{
	//Add Health
	CurrentHealth += DamagePoints;
	//Reset Cooldown
	CurrentCooldownTime = CooldownTimeSec;
	OnUpdateHealth.Broadcast(CurrentHealth);

	UpdateStage();

}

void UHealthCombo::TickUpdate(float DeltaTime)
{
	if (CurrentCooldownTime > 0.f)
	{
		CurrentCooldownTime -= DeltaTime;
		return;
	}
	CurrentHealth = UInterpolationUtil::FAsymptoticAverageSpeedBased(CurrentHealth, DefaultHealth, RestoreToDefaultSpeed, DeltaTime );
	OnUpdateHealth.Broadcast(CurrentHealth);
	UpdateStage();
}

void UHealthCombo::UpdateStage()
{
	//Check if the combo stage is reduced
	int NewStage = FMath::FloorToInt(CurrentHealth/MaxHealth);
	if (NewStage < CurrentStage)
	{
		OnStageDecrease.Broadcast(NewStage);
	}
	if (NewStage > CurrentStage)
	{
		OnStageIncrease.Broadcast(NewStage);
	}
	CurrentStage = NewStage;
}
