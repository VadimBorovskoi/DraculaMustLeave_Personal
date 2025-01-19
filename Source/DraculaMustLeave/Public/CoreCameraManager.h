// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractAim.h"
#include "FreeAim.h"
#include "LockAim.h"
#include "CameraBank.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "TypeUtil.h"
#include "CoreCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DRACULAMUSTLEAVE_API ACoreCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ACoreCameraManager();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sensitivity")
	float PitchSensitivity = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sensitivity")
	float YawSensitivity = 1.f;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UFreeAim> FreeAim;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ULockAim* LockAim;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraBank* Roll;
private:
	UAbstractAim* ActiveAim;
	float InputX;
	float InputY;
	AActor* CurrentTarget;
	float GeneralSensitivity;
	UCameraComponent* ActualCamera;
public:
	UFUNCTION(BlueprintCallable)
	void ProcessInput(float Xinput, float YInput, AActor* PossibleTarget, float SensitivityRate);
	virtual void BeginPlay() override;
	virtual void UpdateCamera(float DeltaTime) override;
	virtual FRotator GetCameraRotation() const override;
private:
	void SetActiveAim(UAbstractAim* Aim, AActor* PossibleTarget = nullptr);
};
