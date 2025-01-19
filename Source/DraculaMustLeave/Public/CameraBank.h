// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"

#include "CameraBank.generated.h"


UCLASS()
class DRACULAMUSTLEAVE_API UCameraBank : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraBank();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	float InterpSpeed = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	float MaxRollValue = 5.0f;
	float CurrentRollValue = 0.0f;
public:
	//Velocity Should be the Dot Product of MovementVelocity and Right Vector
	float GetRoll(float CurrentVelocity, float MaxVelocity,  float DeltaTime, float CameraModifier = 1, float MovementModifier = 1);
private:
	float GetVelPercentile(float CurVel, float MaxVel) {return CurVel / MaxVel;}
};
