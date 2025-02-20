// Fill out your copyright notice in the Description page of Project Settings.


#include "FOV.h"
#include "InterpolationUtil.h"
#include "ReaperPawn.h"
// Sets default values for this component's properties
UFOV::UFOV()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

float UFOV::GetFOV(float DeltaTime, float AdditionalMultiplier)
{
	//Rework so that offsets are interpolated, not the FOV itself
	if (AdditionalMultiplier == 0.0f) AdditionalMultiplier = 1.0f;
		
	if (FMath::IsNearlyEqual(CurrentFOV, TargetFOV.Offset)) return CurrentFOV;

	float Alpha = 0.0f;
	switch (TargetFOV.SmoothingFunction)
	{
	case ESmoothingFunction::Linear:
		Alpha = UInterpolationUtil::FGetInterpolant(CurrentFOV, CurrentStartingFOV, TargetFOV.Offset);
		CurrentFOV = FMath::Lerp(CurrentStartingFOV, TargetFOV.Offset, Alpha) + DeltaTime * TargetFOV.TransitionSmoothness * AdditionalMultiplier;

		break;
	case ESmoothingFunction::Asymptotic:
		CurrentFOV = UInterpolationUtil::FAsymptoticAverageSpeedBased(CurrentFOV, TargetFOV.Offset, TargetFOV.TransitionSmoothness, DeltaTime, AdditionalMultiplier );
		break;
	case ESmoothingFunction::EaseIn:
		CurrentFOV = FMath::InterpEaseIn(CurrentStartingFOV, TargetFOV.Offset,
			UInterpolationUtil::FGetInterpolant(CurrentFOV, CurrentStartingFOV, TargetFOV.Offset) + DeltaTime * TargetFOV.TransitionSmoothness * AdditionalMultiplier, 0.4f);
		break;
	case ESmoothingFunction::EaseOut:
		CurrentFOV = FMath::InterpEaseOut(CurrentStartingFOV, TargetFOV.Offset,
			UInterpolationUtil::FGetInterpolant(CurrentFOV, CurrentStartingFOV, TargetFOV.Offset) + DeltaTime * TargetFOV.TransitionSmoothness * AdditionalMultiplier,
			0.4f);
		break;
	}
	return CurrentFOV;
}

float UFOV::GetInitializedFOV(ULockAim* LockAim, AReaperPawn* Reaper)
{
	LockComponent = LockAim;
	LockComponent->OnActivate.AddUniqueDynamic(this, &UFOV::StartZoomIn);
	LockComponent->OnDeactivate.AddUniqueDynamic(this, &UFOV::StopZoomIn);
	
	Reaper->OnDashBegin.AddUniqueDynamic(this, &UFOV::StartDashNormal);
	Reaper->OnDashEnd.AddUniqueDynamic(this, &UFOV::StopDashNormal);
	Reaper->OnDashUpdate.AddUniqueDynamic(this, &UFOV::StopDashNormal);
	
	TargetFOV = DefaultFOV;
	CurrentStartingFOV = DefaultFOV.Offset;
	CurrentFOV = DefaultFOV.Offset;
	return DefaultFOV.Offset;
}

void UFOV:: StartZoomIn(AActor* Target)
{
	UpdateOffsets(ZoomIn, true);
}

void UFOV:: StopZoomIn(AActor* Target)
{
	UpdateOffsets(ZoomIn, false);
}

void UFOV:: StartDashNormal(AActor* Target, float XValue)
{
	if (FMath::Abs(XValue) <= MaxXValueForDash)
	{
		UpdateOffsets(DashNormal, true);
	}
}
void UFOV:: StopDashNormal(AActor* Target, float Alpha)
{
	if (Alpha>=DashAlphaOverride) UpdateOffsets(DashNormal, false);
}

void UFOV::UpdateOffsets(FOffset TargetOffset, bool Add)
{
	CurrentStartingFOV = TargetFOV.Offset;
	if (!Offsets.Contains(TargetOffset))
	{
		UE_LOG(LogTemp, Warning, TEXT("UFOV::Current Starting FOV %f"), CurrentStartingFOV);
		UE_LOG(LogTemp, Warning, TEXT("UFOV::Target Offset's Offset %f"), TargetOffset.Offset);
	}
	
	Offsets.Remove(TargetOffset);
	if (Add)
	{
		Offsets.AddUnique(TargetOffset);
	}
	TargetFOV = DefaultFOV;
	if (Offsets.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UFOV::Reset Target Offset %f"), TargetFOV.Offset);
		return;
	}
	for (FOffset Offset : Offsets)
	{

		TargetFOV.Offset = FMath::Clamp(TargetFOV.Offset + Offset.Offset, FOVLowerLimit, FOVUpperLimit);
		UE_LOG(LogTemp, Warning, TEXT("UFOV::Updated Target Offset %f"), TargetFOV.Offset);
	}
	
	TargetFOV.TransitionSmoothness = Offsets.Last().TransitionSmoothness;
	TargetFOV.SmoothingFunction = Offsets.Last().SmoothingFunction;
	if (Offsets.Last().bIsAdditive == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Offset is not Additive"));
		TargetFOV.Offset = FMath::Clamp(DefaultFOV.Offset + Offsets.Last().Offset, FOVLowerLimit, FOVUpperLimit);
	}
	UE_LOG(LogTemp, Warning, TEXT("UFOV::UpdateOffsets %d"), Offsets.Num());


}




