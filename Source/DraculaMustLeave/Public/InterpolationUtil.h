// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"  // For debug visualization (optional)
#include "CollisionQueryParams.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"  // For UKismetSystemLibrary::SphereTraceByChannel
#include "Math/UnrealMathUtility.h" 
#include "InterpolationUtil.generated.h"

/**
 * 
 */
UCLASS()
class DRACULAMUSTLEAVE_API UInterpolationUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Asymptotic Average") 
	static FRotator AsymptoticAverageTimeBased(FRotator Value, FRotator TargetRotation, float Time, float DeltaTime, float AdditionalMultiplier = 1.0f);
	UFUNCTION(BlueprintPure, Category = "Asymptotic Average") 
	static FRotator AsymptoticAverageSpeedBased(FRotator Value, FRotator TargetRotation, float Smoothness, float DeltaTime, float AdditionalMultiplier = 1.0f);

	UFUNCTION(BlueprintPure, Category = "Interpolant")
	static float GetInterpolant(FRotator CurVal, FRotator InitVal, FRotator TargetVal);

};
