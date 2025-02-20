// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "Components/ActorComponent.h"
#include "HealthBase.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDie, AActor*, Killer, UObject*, KillingTool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHurt, AActor*, Killer, UObject*, KillingTool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHealth, float, NewHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UHealthBase : public UActorComponent, public IDamageable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthBase();
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnDie OnDie;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnDie OnHurt;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnUpdateHealth OnUpdateHealth;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Values")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Values")
	float LastHitHealth = 0.0f;
	float CurrentHealth = MaxHealth;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void ReceiveDamage(AActor* Sender, UObject* DamageSource, float& Damage, bool& ShouldStopScythe) override;
	virtual void Die(AActor* Sender, UObject* DamageSource) override;
	virtual bool IsDead() override;
};
