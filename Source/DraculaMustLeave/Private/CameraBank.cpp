// Fill out your copyright notice in the Description page of Project Settings.
#include "CameraBank.h"
#include "TypeUtil.h"

// Sets default values for this component's properties
UCameraBank::UCameraBank()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
float UCameraBank::GetRoll(float CurrentVelocity, float MaxVelocity,  float DeltaTime, float CameraModifier, float MovementModifier)
{
	
	float SmoothenedVelocity = UTypeUtil::GetPowWithSign(GetVelPercentile(CurrentVelocity, MaxVelocity), 2);
	float Angle = FMath::Clamp(SmoothenedVelocity * MaxRollValue, -MaxRollValue, MaxRollValue) * CameraModifier * MovementModifier;
	CurrentRollValue = FMath::FInterpTo(CurrentRollValue, Angle, DeltaTime, InterpSpeed);
	return CurrentRollValue;
}
