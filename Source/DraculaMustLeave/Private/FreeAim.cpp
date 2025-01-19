// Fill out your copyright notice in the Description page of Project Settings.


#include "FreeAim.h"
#include "TraceUtil.h"

void UFreeAim::BeginPlay()
{
    Super::BeginPlay();
	CurrentPitchSensitivity= DefaultPitchSensitivity;
	CurrentYawSensitivity= DefaultYawSensitivity;

}
void UFreeAim::HandleAim(float XInput, float YInput, AActor* Target, float SensitivityRate, float DeltaTime, UPARAM(ref) float& TargetPitch, UPARAM(ref) float& TargetYaw)
{
	TArray<FHitResult> Hits = CheckSoftAim();
	bool bTargetHit = Hits.IsEmpty() == false;
	if (bTargetHit)
	{
		if (bSoftAimTargetFound != bTargetHit) FOnSoftAimTargetFound();
		CurrentPitchSensitivity = FMath::FInterpTo(CurrentPitchSensitivity, DefaultPitchSensitivity * SoftAimSensitivity, DeltaTime, SensitivityInterpSpeed);
		CurrentYawSensitivity =  FMath::FInterpTo(CurrentYawSensitivity, DefaultYawSensitivity * SoftAimSensitivity, DeltaTime, SensitivityInterpSpeed);
	} else
	{
		if (bSoftAimTargetFound != bTargetHit) FOnSoftAimRelease();
		CurrentPitchSensitivity = FMath::FInterpTo(CurrentPitchSensitivity, DefaultPitchSensitivity, DeltaTime, SensitivityInterpSpeed);
		CurrentYawSensitivity =  FMath::FInterpTo(CurrentYawSensitivity, DefaultYawSensitivity, DeltaTime, SensitivityInterpSpeed);
	}
	TargetPitch += (YInput * CurrentPitchSensitivity) * DeltaTime;
	TargetPitch = FMath::Clamp(TargetPitch, MinPitch, MaxPitch);
	TargetYaw += (XInput * CurrentYawSensitivity) * DeltaTime;
}




