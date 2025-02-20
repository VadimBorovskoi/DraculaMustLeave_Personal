// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TypeUtil.generated.h"

/**
 * 
 */
UCLASS()
class DRACULAMUSTLEAVE_API UTypeUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static float GetRotatorDistance(FRotator Start, FRotator End);
	static float GetPowWithSign(float num, float pow);
	static UActorComponent* GetFirstComponentByInterface(AActor* Actor, TSubclassOf<UInterface> InterfaceClass);
};
