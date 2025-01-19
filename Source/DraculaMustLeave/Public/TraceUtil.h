// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"  // For debug visualization (optional)
#include "CollisionQueryParams.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"  // For UKismetSystemLibrary::SphereTraceByChannel
#include "Math/UnrealMathUtility.h" // For FMath functions
#include "TraceUtil.generated.h"


/**
 * 
 */
UENUM(BlueprintType)
enum class EConeDebugShape : uint8 {
	Cone = 0 UMETA(DisplayName = "Cone"),
	Circle = 1 UMETA(DisplayName = "Circle")
};


UCLASS()
class DRACULAMUSTLEAVE_API UTraceUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "ConeTrace")
    static TArray<FHitResult> ConeTraceByChannel(
        UObject* WorldContextObject,
        const FVector& Start,
        const FVector& End,
        float Radius,
        ETraceTypeQuery TraceChannel,
        bool bTraceComplex,
        const TArray<AActor*>& ActorsToIgnore,
        EDrawDebugTrace::Type DrawDebugType,
        bool bIgnoreSelf,
        EConeDebugShape DebugShape,
        FColor TraceColor,
        FColor TraceHitColor,
        float DebugDuration
    );

private:
    static bool ConeTraceChannelLogic(UObject* WorldContextObject,
        const FVector& Start,
        const FVector& End,
        float Radius,
        ETraceTypeQuery TraceChannel,
        bool bTraceComplex,
        const TArray<AActor*>& ActorsToIgnore,
        bool bIgnoreSelf,
        TArray<FHitResult>& OutHits,
        float& OutAngleRad,
        float& OutDistance,
        FVector& OutDirection
        );
};
