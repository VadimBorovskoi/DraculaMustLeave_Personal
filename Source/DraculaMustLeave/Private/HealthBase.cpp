// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBase.h"
// Sets default values for this component's properties
UHealthBase::UHealthBase()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	// ...
}

void UHealthBase::ReceiveDamage(AActor* Sender, UObject* DamageSource, const FHitResult& SweepResult, float& Damage, bool& ShouldStopScythe)
{
	if (MaxHealth != 0)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
		CurrentHealth = CurrentHealth < LastHitHealth ? LastHitHealth : CurrentHealth;
	}
	if (ShouldStopScythe) ShouldStopScythe = CurrentHealth > 0.0f || MaxHealth == 0.f;
	UE_LOG(LogTemp, Warning, TEXT("Health Left %f"), CurrentHealth);
	OnUpdateHealth.Broadcast(CurrentHealth, -1);
	if (IsDead())
	{
		if(GetOwner()->ActorHasTag(FName("Enemy")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Death Damage Points"));
			Damage = 60.f;
		}
		Die(Sender, DamageSource);
	}
	else
	{
		OnHurt.Broadcast(Sender, DamageSource, SweepResult);
	}
}
void UHealthBase::Die(AActor* Sender, UObject* DamageSource)
{
	UE_LOG(LogTemp, Warning, TEXT("Dead"));
	OnDie.Broadcast(Sender, DamageSource);
	GetOwner()->Destroy();
}

bool UHealthBase::IsDead()
{
	return CurrentHealth <= 0.f && MaxHealth != 0.f;
}
