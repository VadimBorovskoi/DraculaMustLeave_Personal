// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractAim.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"
#include "TypeUtil.h"

#include "LockAim.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockTransition, float, Interpolant, AActor*, ActorTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetLocked, AActor*, ActorTarget);

UCLASS()
class DRACULAMUSTLEAVE_API ULockAim : public UAbstractAim
{
	GENERATED_BODY()
public:
	ULockAim();

	AActor* Owner;
	
	UPROPERTY(BlueprintAssignable, Category="Event Dispatchers")
	FOnLockTransition OnLockTransition;
	UPROPERTY(BlueprintAssignable, Category="Event Dispatchers")
	FOnTargetLocked OnTargetLocked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed Definitions")
	float LockRotationRate = .4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed Definitions")
	float GeneralRotationRate = .8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed Definitions")
	float MaxRotationTime = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Offset Params")
	float MaxPitchAdd = 7.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Offset Params")
	float MaxYawAdd = 7.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Offset Params")
	float OffsetRotationRate = .45f;

private:
	bool bIsLocked = false;
	FRotator StartingRotation;
	float LockTolerance = 5.0f;

public:
	virtual void HandleAim(float XInput, float YInput, AActor* Target, float SensitivityRate, float DeltaTime, float& TargetPitch, float& TargetYaw) override;
private:
	UFUNCTION()
	void Reset(AActor* Target);
	bool IsLookingAt(FRotator LookAtRotation);
};
