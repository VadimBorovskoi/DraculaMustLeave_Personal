// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Pushable.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPush, FVector, ImpactPosition, FVector, Direction, float, Force );
/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UPushable : public UInterface
{
	GENERATED_BODY()
};
class DRACULAMUSTLEAVE_API IPushable
{
	GENERATED_BODY()

public:
	virtual void  ApplyForce(const FVector ImpactPosition, const FVector Direction, const float Force);
	virtual void  UpdateOwner(float DeltaTime);
	virtual void ResetForce();
};
