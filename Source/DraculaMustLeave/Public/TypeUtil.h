// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Framework/Application/SlateApplication.h"
#include "InputCoreTypes.h"
#include "TypeUtil.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGenericDelegate);



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionActivate, float, XDir, FVector, TargetPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionDeactivate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharge, float, ElapsedTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnColliderOverlap,UPrimitiveComponent*, OverlappedComponent, 
			AActor*, OtherActor, 
			UPrimitiveComponent*, OtherComp, 
			int32, OtherBodyIndex, 
			bool, bFromSweep, 
			const FHitResult&, SweepResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnMeshOverlap,UPrimitiveComponent*, OverlappedComponent, 
			AActor*, OtherActor, 
			UPrimitiveComponent*, OtherComp, 
			int32, OtherBodyIndex, 
			bool, bFromSweep, 
			const FHitResult&, SweepResult);

USTRUCT(BlueprintType)
struct FScytheActionParameters
{
	GENERATED_BODY()
public:
	FScytheActionParameters()
		: bShouldStopAtAnObstacle(true),
		  bShouldStopAfterHit(true),
		  MaxVelocity(0.0f),
		  MinVelocity(0.0f),
		  AccelerationCurve(nullptr),
		  DecelerationCurve(nullptr),
		  RotationRate(0.0f),
		  RollAngle(0.0f),
		  SpinSign(1),
		  ManaConsumption(0.0f),
		  ManaConsumptionPerFrame(0.0f),
		  DamagePerHit(0.0f)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Parameters")
	bool bShouldStopAtAnObstacle = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Parameters")
	bool bShouldStopAfterHit = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Parameters")
	float MaxVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Parameters")
	float MinVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Parameters")
	UCurveFloat* AccelerationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Parameters")
	UCurveFloat* DecelerationCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Parameters")
	float RotationRate;
	//int RollSign;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Parameters")
	float RollAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Parameters")
	int SpinSign;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Parameters")
	float ManaConsumption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Parameters")
	float ManaConsumptionPerFrame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Parameters")
	float DamagePerHit;

	FScytheActionParameters operator+(const FScytheActionParameters& Other) const
	{
		FScytheActionParameters Result;

		// Make sure the Scythe goes through obstacles if any ability allows it 
		Result.bShouldStopAtAnObstacle = bShouldStopAtAnObstacle && Other.bShouldStopAtAnObstacle;
		Result.bShouldStopAfterHit = bShouldStopAfterHit && Other.bShouldStopAfterHit;

		// Addition for numeric fields
		Result.MaxVelocity = MaxVelocity + Other.MaxVelocity;
		Result.MinVelocity = MinVelocity + Other.MinVelocity;
		Result.RotationRate = RotationRate + Other.RotationRate;
		Result.RollAngle = RollAngle + Other.RollAngle;
		Result.SpinSign = FMath::Sign(SpinSign * Other.SpinSign); 
		Result.ManaConsumption = ManaConsumption + Other.ManaConsumption;
		Result.ManaConsumptionPerFrame = ManaConsumptionPerFrame + Other.ManaConsumptionPerFrame;
		Result.DamagePerHit = DamagePerHit + Other.DamagePerHit;

		// Pointer fields are not added (could be handled separately if needed)
		Result.AccelerationCurve = Other.AccelerationCurve ? Other.AccelerationCurve : AccelerationCurve;
		Result.DecelerationCurve = Other.DecelerationCurve ? Other.DecelerationCurve : DecelerationCurve;

		return Result;
	}

	FScytheActionParameters& operator+=(const FScytheActionParameters& Other)
	{
		*this = *this + Other;
		return *this;
	}
	void SetNonZeroParams(const FScytheActionParameters& Other)
	{
		bShouldStopAtAnObstacle = Other.bShouldStopAtAnObstacle;
		bShouldStopAfterHit = Other.bShouldStopAfterHit;

		// Update numeric fields only if Other has non-zero values
		if (Other.MaxVelocity != 0.0f) MaxVelocity = Other.MaxVelocity;
		if (Other.MinVelocity != 0.0f) MinVelocity = Other.MinVelocity;
		if (Other.RotationRate != 0.0f) RotationRate = Other.RotationRate;
		if (Other.RollAngle != 0.0f) RollAngle = Other.RollAngle;
		if (Other.SpinSign != 0) SpinSign = Other.SpinSign;
		if (Other.ManaConsumption != 0.0f) ManaConsumption = Other.ManaConsumption;
		if (Other.ManaConsumptionPerFrame != 0.0f) ManaConsumptionPerFrame = Other.ManaConsumptionPerFrame;
		if (Other.DamagePerHit != 0.0f) DamagePerHit = Other.DamagePerHit;

		// Assign pointer fields only if Other has a valid value
		if (Other.AccelerationCurve) AccelerationCurve = Other.AccelerationCurve;
		if (Other.DecelerationCurve) DecelerationCurve = Other.DecelerationCurve;
	}
	bool operator==(const FScytheActionParameters& Other) const
	{
		return bShouldStopAtAnObstacle == Other.bShouldStopAtAnObstacle &&
			   bShouldStopAfterHit == Other.bShouldStopAfterHit &&
			   MaxVelocity == Other.MaxVelocity &&
			   MinVelocity == Other.MinVelocity &&
			   RotationRate == Other.RotationRate &&
			   RollAngle == Other.RollAngle &&
			   SpinSign == Other.SpinSign &&
			   ManaConsumption == Other.ManaConsumption &&
			   ManaConsumptionPerFrame == Other.ManaConsumptionPerFrame &&
			   DamagePerHit == Other.DamagePerHit &&
			   AccelerationCurve == Other.AccelerationCurve && // Pointer comparison
			   DecelerationCurve == Other.DecelerationCurve;   // Pointer comparison
	}
	bool operator!=(const FScytheActionParameters& Other) const
	{
		return !(*this == Other);
	}
};

UCLASS()
class DRACULAMUSTLEAVE_API UTypeUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static float GetRotatorDistance(FRotator Start, FRotator End);
	static float GetPowWithSign(float num, float pow);
	static UActorComponent* GetFirstComponentByInterface(AActor* Actor, TSubclassOf<UInterface> InterfaceClass);
	UFUNCTION(BlueprintPure, Category = "Time Related")
	static void GetMinutesAndSeconds(int32 TotalSeconds, int32& OutMinutes, int32& OutSeconds );
	static FVector2D GetRandomVector2DInRange(const FVector2D& Min, const FVector2D& Max);

};
