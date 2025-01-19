// Fill out your copyright notice in the Description page of Project Settings.


#include "TypeUtil.h"

float UTypeUtil::GetRotatorDistance(FRotator Start, FRotator End)
{
	FQuat QStart = FQuat(Start);
	FQuat QEnd = FQuat(End);

	return FMath::Abs(QStart.AngularDistance(QEnd));
}
float UTypeUtil::GetPowWithSign(float num, float pow)
{
	return FMath::Sign(num) * FMath::Pow(num, pow);
}