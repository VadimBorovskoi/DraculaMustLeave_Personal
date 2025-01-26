// Fill out your copyright notice in the Description page of Project Settings.


#include "InterpolationUtil.h"
#include "TypeUtil.h"

float UInterpolationUtil:: GetInterpolant(FRotator CurVal, FRotator InitVal, FRotator TargetVal)
{
	return UTypeUtil::GetRotatorDistance(CurVal, TargetVal)/UTypeUtil::GetRotatorDistance(TargetVal,InitVal);
}
float UInterpolationUtil::FGetInterpolant(float CurVal, float InitVal, float TargetVal)
{
	if (FMath::IsNearlyEqual(InitVal, TargetVal)) return 0.0f;
	return FMath::Clamp((CurVal - InitVal) / (TargetVal - InitVal), 0.0f, 1.0f);
}
FRotator UInterpolationUtil :: AsymptoticAverageTimeBased(FRotator Value, FRotator TargetRotation, float Time, float DeltaTime, float AdditionalMultiplier)
{
	float Smoothness = -FMath::Loge(0.01)/Time;
	return AsymptoticAverageSpeedBased(Value, TargetRotation, Smoothness, DeltaTime, AdditionalMultiplier);
}
FRotator UInterpolationUtil :: AsymptoticAverageSpeedBased(FRotator Value, FRotator TargetRotation, float Smoothness, float DeltaTime, float AdditionalMultiplier)
{
	// Normalize the difference for shortest rotation
	float DeltaYaw = FMath::FindDeltaAngleDegrees(Value.Yaw, TargetRotation.Yaw);
	float DeltaPitch = FMath::FindDeltaAngleDegrees(Value.Pitch, TargetRotation.Pitch);
	float DeltaRoll = FMath::FindDeltaAngleDegrees(Value.Roll, TargetRotation.Roll);

	// Apply asymptotic smoothing to each axis
	FRotator SmoothedRotation;
	SmoothedRotation.Yaw = FAsymptoticAverageByDiff(Value.Yaw, DeltaYaw, Smoothness, DeltaTime, AdditionalMultiplier);
	SmoothedRotation.Pitch = FAsymptoticAverageByDiff(Value.Pitch, DeltaPitch, Smoothness, DeltaTime, AdditionalMultiplier);
	SmoothedRotation.Roll = FAsymptoticAverageByDiff(Value.Roll, DeltaRoll, Smoothness, DeltaTime, AdditionalMultiplier);
	
	return SmoothedRotation;

}

float UInterpolationUtil::FAsymptoticAverageSpeedBased(float Value, float target, float Smoothness, float DeltaTime, float AdditionalMultiplier)
{
	return 	 Value + (target - Value)  * FMath::Min(1.0f, DeltaTime * Smoothness) * AdditionalMultiplier;
}
float UInterpolationUtil:: FAsymptoticAverageByDiff(float Value, float Diff, float Smoothness, float DeltaTime, float AdditionalMultiplier)
{
	return Value + Diff * FMath::Min(1.0f, DeltaTime * Smoothness) * AdditionalMultiplier;
}



