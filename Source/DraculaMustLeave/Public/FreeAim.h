// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractAim.h"
#include "FreeAim.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoftAimTargetFound, AActor*, ActorTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSoftAimRelease);

UCLASS()
class DRACULAMUSTLEAVE_API UFreeAim : public UAbstractAim
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category="Event Dispatchers")
	FOnSoftAimTargetFound OnSoftAimTargetFound;
	UPROPERTY(BlueprintAssignable, Category="Event Dispatchers")
	FOnSoftAimRelease OnSoftAimRelease;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sensitivity")
	float DefaultPitchSensitivity = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sensitivity")
	float DefaultYawSensitivity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoftAim")
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoftAim")
	float SoftAimDistance = 6000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoftAim")
	float SoftAimRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoftAim")
	float SoftAimSensitivity = .5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoftAim")
	float SensitivityInterpSpeed = 5.f;
	
	bool bSoftAimTargetFound = false;
	float CurrentPitchSensitivity;
	float CurrentYawSensitivity;
	

	
public:
	//virtual void HandleAim(float XInput, float YInput, float SensitivityRate, float& TargetPitch, float& TargetYaw) override;
	virtual void BeginPlay() override;
	virtual void HandleAim(float XInput, float YInput, AActor* Target, float SensitivityRate, float DeltaTime, float& TargetPitch, float& TargetYaw) override;
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SoftAim")
	TArray<FHitResult> CheckSoftAim();
};
