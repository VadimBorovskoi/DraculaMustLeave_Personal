// Fill out your copyright notice in the Description page of Project Settings.


#include "TypeUtil.h"

#include "AbsScytheAction.h"

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
UActorComponent* UTypeUtil::GetFirstComponentByInterface(AActor* Actor, TSubclassOf<UInterface> InterfaceClass)
{
	if (!Actor || !InterfaceClass)
	{
		return nullptr;
	}

	TArray<UActorComponent*> Components = Actor->GetComponentsByInterface(InterfaceClass);
    
	return Components.Num() > 0 ? Components[0] : nullptr;
}
