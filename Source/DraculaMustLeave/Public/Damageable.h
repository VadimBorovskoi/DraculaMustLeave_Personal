// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Damageable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class DRACULAMUSTLEAVE_API IDamageable
{
	GENERATED_BODY()

public:
	virtual void  ReceiveDamage(AActor* Sender, UObject* DamageSource, float &Damage, bool& ShouldStopScythe);
	virtual void  Die(AActor* Sender, UObject* DamageSource);
	virtual bool  IsDead();

};

