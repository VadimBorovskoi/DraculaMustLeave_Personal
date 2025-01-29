// Fill out your copyright notice in the Description page of Project Settings.


#include "StrafeMovement.h"
void UStrafeMovement::HandleMovement(float XInput, float YInput, AActor* Target, float& XScale, float& YScale, FVector& XDirection, FVector& YDirection)
{
	if (Target == nullptr || Target->IsValidLowLevel() == false) return;
	FVector ForwardDirection = Target->GetActorLocation() - GetOwner()->GetActorLocation();
	FVector RightDirection = FVector::CrossProduct(ForwardDirection, GetOwner()->GetActorUpVector());
	XScale = -XInput;
	YScale = YInput;
	XDirection = RightDirection.GetSafeNormal();
	YDirection = ForwardDirection.GetSafeNormal();
}
void UStrafeMovement::InterpDash(AActor* Target, FVector StartPosition, FVector TargetPosition, float Alpha, FVector& NewPosition, bool& Successful)
{
	
	if (CharacterOwner == nullptr || Target == nullptr)
	{
		Successful = false;
		return;
	}
	if (!bIsStrafing)
	{
		NewPosition = FMath::Lerp(StartPosition, TargetPosition, Alpha);

		UCapsuleComponent* CapsuleComponent = CharacterOwner->GetCapsuleComponent();
		float Radius = CapsuleComponent->GetUnscaledCapsuleRadius();
		float HalfHeight = CapsuleComponent->GetUnscaledCapsuleHalfHeight();

		Successful = !IsDashBlocked(NewPosition, Radius, HalfHeight, DashCollisionChannel);
		return;
	}
	// Ensure movement stays on the X-Y plane
	FVector TargetPosition2D = TargetPosition;
	TargetPosition2D.Z = StartPosition.Z; // Ensure target is on the same plane as StartPosition

	// Compute center of the arc (same logic as GetDashTargetPosition)
	FVector Center = Target->GetActorLocation();
	Center.Z = StartPosition.Z; // Keep the center in the same plane as StartPosition

	// Compute initial radius
	float Radius = FVector::Distance(StartPosition, Center);

	// Find current and target angles relative to the center
	FVector ToStart = (StartPosition - Center).GetSafeNormal();
	FVector ToEnd = (TargetPosition2D - Center).GetSafeNormal();

	float StartAngle = FMath::Atan2(ToStart.Y, ToStart.X);
	float EndAngle = FMath::Atan2(ToEnd.Y, ToEnd.X);

	// Ensure shortest rotation direction
	float AngleDifference = EndAngle - StartAngle;
	if (AngleDifference > PI)
		EndAngle -= 2 * PI;
	else if (AngleDifference < -PI)
		EndAngle += 2 * PI;

	// Interpolate the angle
	float InterpolatedAngle = FMath::Lerp(StartAngle, EndAngle, Alpha);

	// Compute new position along the arc, keep Z constant
	NewPosition = Center + FVector(FMath::Cos(InterpolatedAngle) * Radius, FMath::Sin(InterpolatedAngle) * Radius, StartPosition.Z);
	NewPosition.Z = StartPosition.Z;
	// Collision check
	UCapsuleComponent* CapsuleComponent = CharacterOwner->GetCapsuleComponent();
	float RadiusCapsule = CapsuleComponent->GetUnscaledCapsuleRadius();
	float HalfHeight = CapsuleComponent->GetUnscaledCapsuleHalfHeight();

	Successful = !IsDashBlocked(NewPosition, RadiusCapsule, HalfHeight, DashCollisionChannel);
}

void UStrafeMovement::GetDashTargetPosition(AActor* Target, float XInput, float YInput, FVector& TargetPosition, bool& Successful)
{
	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner == nullptr || Target == nullptr)
	{
		Successful = false;
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("XInput: %f"), XInput);
	if (FMath::Abs(XInput) < .25f){
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
		bIsStrafing = false;
		return;
	}
	// Get current character position and target position
	FVector StartPosition = CharacterOwner->GetActorLocation();
	FVector TargetPosition2D = Target->GetActorLocation();

	// Compute initial radius (distance from character to target)
	float Radius = FVector::Distance(StartPosition, TargetPosition2D);

	
	Radius += YInput * YDashScale; // Decrease radius (move inward)
    
	Radius = FMath::Max(Radius, 10.0f); // Prevent radius from becoming too small

	// Compute current angle of the character around the target
	FVector ToCharacter = (StartPosition - TargetPosition2D).GetSafeNormal();
	float StartAngle = FMath::Atan2(ToCharacter.Y, ToCharacter.X);

	// Compute arc distance based on DashDistance
	float ArcLength = DashDistance; // We assume DashDistance is the travel distance along the arc
	float ArcAngle = ArcLength / Radius * FMath::Sign(-XInput); // θ = s / r (arc length formula)
	
    
	float TargetAngle = StartAngle + ArcAngle;

	// Compute new target position along the arc
	TargetPosition = TargetPosition2D + FVector(FMath::Cos(TargetAngle) * Radius, FMath::Sin(TargetAngle) * Radius, StartPosition.Z);
	TargetPosition.Z = StartPosition.Z;
	DrawDebugSphere(GetWorld(), TargetPosition, 36, 12, FColor::Yellow, true, 40.0f);

	bIsStrafing = true;
	Successful = true;
}
