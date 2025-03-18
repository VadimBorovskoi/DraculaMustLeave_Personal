// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Updatable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUpdatable : public UInterface
{
	GENERATED_BODY()
};

class DRACULAMUSTLEAVE_API IUpdatable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void  TickUpdate(float DeltaTime);
};