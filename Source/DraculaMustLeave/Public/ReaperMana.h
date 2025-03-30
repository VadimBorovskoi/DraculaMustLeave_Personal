// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBase.h"
#include "Updatable.h"
#include "Components/ActorComponent.h"
#include "ReaperMana.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UReaperMana : public UActorComponent, public IUpdatable
{
	GENERATED_BODY()
protected:
	float CurrentMana = MaxMana;
	float CurrentGhostMana = MaxMana;
	float CurRegenCooldownTime = 0.f;
	float CurGhostManaCooldownTime = 0.f;
	bool bShouldManaRegen = false;
	bool bShouldGhostManaDrop = false;
public:	
	// Sets default values for this component's properties
	//When Mana is reduced, ghost mana's current parameter is reduced
	UReaperMana();
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnUpdateStats OnUpdateMana;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnUpdateStats OnUpdateGhostMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Parameters")
	float MaxMana = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Parameters")
	float ManaRegenMinVelocity = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Parameters")
	float ManaRegenMaxVelocity = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Parameters")
	float ManaRegenCooldownTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Parameters")
	UCurveFloat* ManaRegenAccelerationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Mana Parameters")
	float GhostManaMinVelocity = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Mana Parameters")
	float GhostManaMaxVelocity = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Mana Parameters")
	float GhostManaCooldownTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Mana Parameters")
	UCurveFloat* GhostManaAccelerationCurve;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickUpdate(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual void ReduceMana(float DeductedAmount, bool bShouldDropGhostManaAlong);
	UFUNCTION(BlueprintCallable)
	virtual void RestoreMana(float AddedAmount);
	UFUNCTION(BlueprintCallable)
	virtual bool CanUseAbility(float AbilityMana);
protected:
	UFUNCTION(BlueprintCallable)
	virtual bool TryReduceGhostMana(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	virtual bool TryRegenerateMana(float DeltaTime);
	
};
