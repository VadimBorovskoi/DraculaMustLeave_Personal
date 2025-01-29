// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbstractAim.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivate, AActor*, ActorTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeactivate, AActor*, ActorTarget);

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UAbstractAim : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbstractAim();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnActivate OnActivate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnDeactivate OnDeactivate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pitch Limits")
	float MaxPitch = +85.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pitch Limits")
	float MinPitch = -85.0f;
	
	UFUNCTION(BlueprintPure, Category = "Abstract")
	virtual void HandleAim(float XInput, float YInput,AActor* Target, float SensitivityRate, float DeltaTime, UPARAM(ref) float& TargetPitch, UPARAM(ref) float& TargetYaw) PURE_VIRTUAL(0);
};
