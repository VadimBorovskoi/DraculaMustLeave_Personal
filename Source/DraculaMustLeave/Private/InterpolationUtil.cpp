// Fill out your copyright notice in the Description page of Project Settings.


#include "InterpolationUtil.h"
#include "TypeUtil.h"

float UInterpolationUtil:: GetInterpolant(FRotator CurVal, FRotator InitVal, FRotator TargetVal)
{
	return UTypeUtil::GetRotatorDistance(CurVal, TargetVal)/UTypeUtil::GetRotatorDistance(TargetVal,InitVal);
}

FRotator UInterpolationUtil :: AsymptoticAverageTimeBased(FRotator Value, FRotator TargetRotation, float Time, float DeltaTime, float AdditionalMultiplier)
{
	float Smoothness = -FMath::Loge(0.01)/Time;
	return AsymptoticAverageSpeedBased(Value, TargetRotation, Smoothness, DeltaTime, AdditionalMultiplier);
}
FRotator UInterpolationUtil :: AsymptoticAverageSpeedBased(FRotator Value, FRotator TargetRotation, float Smoothness, float DeltaTime, float AdditionalMultiplier)
{
	return Value + (TargetRotation - Value) * Smoothness * FMath::Min(1.0, DeltaTime * Smoothness) * AdditionalMultiplier;
}
