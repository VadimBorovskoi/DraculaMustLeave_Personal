// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCombo.h"
#include "InterpolationUtil.h"

UHealthCombo::UHealthCombo()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UHealthCombo::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = DefaultHealth;
}

void UHealthCombo::ReceiveDamage(AActor* Sender, UObject* DamageSource, float& Damage, bool& ShouldStopScythe)
{
	CurrentHealth = FMath::Min(0.f, CurrentHealth - Damage);
	CurrentHealth = CurrentHealth < LastHitHealth ? LastHitHealth : CurrentHealth;
	if (ShouldStopScythe) ShouldStopScythe = CurrentHealth > 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("Health Left %f"), CurrentHealth);
	CurrentCooldownTime = CooldownTimeSec;
	OnUpdateHealth.Broadcast(CurrentHealth);
	if (CurrentHealth <= 0.f)
	{
		Die(Sender, DamageSource);
		return;
	}
	OnHurt.Broadcast(Sender, DamageSource);
	
}
void UHealthCombo::Die(AActor* Sender, UObject* DamageSource)
{
	Super::Die(Sender, DamageSource);
}
void UHealthCombo::AddHealth(float DamagePoints)
{
	CurrentHealth += DamagePoints;
	CurrentCooldownTime = CooldownTimeSec;
	OnUpdateHealth.Broadcast(CurrentHealth);
}
void UHealthCombo::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (CurrentCooldownTime > 0.f)
	{
		CurrentCooldownTime -= DeltaTime;
		return;
	}
	CurrentHealth = UInterpolationUtil::FAsymptoticAverageSpeedBased(CurrentHealth, DefaultHealth, RestoreToDefaultSpeed, DeltaTime );
	OnUpdateHealth.Broadcast(CurrentHealth);
}
