// Fill out your copyright notice in the Description page of Project Settings.


#include "LockAim.h"
#include "InterpolationUtil.h"

ULockAim::ULockAim()
{
	Owner = GetOwner();
	OnActivate.AddUniqueDynamic(this, &ULockAim::Reset);
}

void ULockAim::HandleAim(float XInput, float YInput, AActor* Target, float SensitivityRate, float DeltaTime, UPARAM(ref) float& TargetPitch, UPARAM(ref) float& TargetYaw)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), Target->GetActorLocation());
	bool bLocked = IsLookingAt(LookAtRotation);

	if (bLocked && !bIsLocked)
	{
		bIsLocked = true;
		OnTargetLocked.Broadcast(Target);
	} 

	FRotator NewRotation;
	if (bIsLocked)
	{

		NewRotation = UInterpolationUtil::AsymptoticAverageSpeedBased(Owner->GetActorRotation(), LookAtRotation, GeneralRotationRate,  DeltaTime);
		//Add Offset
		LookAtRotation = NewRotation;
		LookAtRotation.Pitch = FMath::Clamp(LookAtRotation.Pitch + MaxPitchAdd * YInput,
			MinPitch > LookAtRotation.Pitch ? LookAtRotation.Pitch : MinPitch, MaxPitch < LookAtRotation.Pitch ? LookAtRotation.Pitch : MaxPitch);
		LookAtRotation.Yaw += MaxYawAdd * XInput;
		NewRotation = UInterpolationUtil::AsymptoticAverageSpeedBased(NewRotation, LookAtRotation, OffsetRotationRate,  DeltaTime);
	}
	else
	{
		float RotationDistance = UTypeUtil::GetRotatorDistance(Owner->GetActorRotation(), LookAtRotation);
		//Finding the time within which the interpolation has to take place
		float DesiredTime = FMath::Clamp(0.1, -FMath::Loge(0.01)/LockRotationRate, MaxRotationTime );
		NewRotation = UInterpolationUtil::AsymptoticAverageTimeBased(Owner->GetActorRotation(), LookAtRotation,
			DesiredTime,DeltaTime, FMath::Pow( SensitivityRate,2 ));
		OnLockTransition.Broadcast(UInterpolationUtil::GetInterpolant(NewRotation, StartingRotation, LookAtRotation), Target);
	}
	TargetPitch = NewRotation.Pitch;
	TargetYaw = NewRotation.Yaw;
	
}
void ULockAim::Reset(AActor* Target)
{
	StartingRotation = GetOwner()->GetActorRotation();
	bIsLocked = false;
}
bool ULockAim::IsLookingAt(FRotator LookAtRotation)
{
	FRotator DeltaRotation = (LookAtRotation - Owner -> GetActorRotation()).GetNormalized();
	return FMath::Abs(DeltaRotation.Pitch) <= LockTolerance && FMath::Abs(DeltaRotation.Yaw) <= LockTolerance;
}

