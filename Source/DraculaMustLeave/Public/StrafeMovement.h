// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractMovement.h"
#include "StrafeMovement.generated.h"

/**
 * 
 */
UCLASS()
class DRACULAMUSTLEAVE_API UStrafeMovement : public UAbstractMovement
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafe Dash")
	float YDashScale = 5.0f;
private:
	bool bIsStrafing = false;
	int XSign;
public:
	virtual void HandleMovement(float XInput, float YInput,AActor* Target,
		UPARAM(ref) float& XScale, UPARAM(ref) float& YScale, UPARAM(ref) FVector& XDirection, UPARAM(ref) FVector& YDirection) override;
	virtual void InterpDash(AActor* Target, FVector StartPosition, FVector TargetPosition, float Alpha, FVector& NewPosition, bool& Successful) override;
	virtual void GetDashTargetPosition(AActor* Target, float XInput, float YInput, FVector& TargetPosition, bool& Successful) override;
};
