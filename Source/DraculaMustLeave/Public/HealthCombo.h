// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBase.h"
#include "HealthCombo.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UHealthCombo : public UHealthBase
{
	GENERATED_BODY()
public:
	UHealthCombo();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComboBarParameters")
	float DefaultHealth = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Values")
	float CooldownTimeSec = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Values")
	float RestoreToDefaultSpeed = 2.f;

	float CurrentCooldownTime = 0.f;
protected:
	virtual void BeginPlay() override;
public:
	virtual void ReceiveDamage(AActor* Sender, UObject* DamageSource, float& Damage, bool& ShouldStopScythe) override;
	virtual void Die(AActor* Sender, UObject* DamageSource) override;
	void AddHealth(float DamagePoints);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
