// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBase.h"
#include "Updatable.h"
#include "HealthCombo.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageChange, int, Stage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UHealthCombo : public UHealthBase, public IUpdatable
{
	GENERATED_BODY()
public:
	UHealthCombo();
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnStageChange OnStageDecrease;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnStageChange OnStageIncrease;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComboBarParameters")
	float DefaultHealth = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Values")
	float CooldownTimeSec = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Values")
	float RestoreToDefaultSpeed = 2.f;
	
	float CurrentCooldownTime = 0.f;
	int CurrentStage = 0;
public:
	virtual void ReceiveDamage(AActor* Sender, UObject* DamageSource, float& Damage, bool& ShouldStopScythe) override;
	virtual void Die(AActor* Sender, UObject* DamageSource) override;
	virtual void TickUpdate(float DeltaTime) override;
	float GetDefaultHealth() { return DefaultHealth; }
	void AddHealth(float DamagePoints);
	UFUNCTION(BlueprintPure, Category="Health Data")
	int GetCurrentStage() { return CurrentStage; }
protected:
	virtual void BeginPlay() override;
	virtual void UpdateStage();
};
