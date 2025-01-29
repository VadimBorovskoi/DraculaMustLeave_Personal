// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMovement.h"


void UBaseMovement::HandleMovement(float XInput, float YInput, AActor* Target, float& XScale, float& YScale, FVector& XDirection, FVector& YDirection)
{
	XScale = XInput;
	YScale = YInput;
	XDirection = GetOwner()->GetActorRightVector();
	YDirection = GetOwner()->GetActorForwardVector();
}
void UBaseMovement::InterpDash(AActor* Target, FVector StartPosition, FVector TargetPosition, float Alpha, FVector& NewPosition, bool& Successful)
{
	NewPosition = FMath::Lerp(StartPosition, TargetPosition, Alpha);

	UCapsuleComponent* CapsuleComponent = CharacterOwner->GetCapsuleComponent();
	float Radius = CapsuleComponent->GetUnscaledCapsuleRadius();
	float HalfHeight = CapsuleComponent->GetUnscaledCapsuleHalfHeight();

	Successful = !IsDashBlocked(NewPosition, Radius, HalfHeight, DashCollisionChannel);
}
void UBaseMovement::GetDashTargetPosition(AActor* Target, float XInput, float YInput, FVector& TargetPosition, bool& Successful)
{
	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner == nullptr)
	{
		Successful = false;
		return;
	}
	// Get movement vectors
	FVector ForwardVector = CharacterOwner->GetActorForwardVector();
	FVector RightVector = CharacterOwner->GetActorRightVector();

	// Compute dash direction
	FVector DashDirection = (ForwardVector * (YInput==0 ? 1 : YInput)) + (RightVector * (XInput ==0 ? 0 : XInput));
	DashDirection = DashDirection.GetSafeNormal();

	// Compute target position
	FVector StartPosition = CharacterOwner->GetActorLocation();
	TargetPosition = StartPosition + (DashDirection * DashDistance);

	Successful = true;
}


