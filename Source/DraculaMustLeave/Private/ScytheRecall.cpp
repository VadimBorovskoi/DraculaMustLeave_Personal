// Fill out your copyright notice in the Description page of Project Settings.


#include "ScytheRecall.h"

#include "InterpolationUtil.h"
#include "Kismet/KismetMathLibrary.h"

//Can Switch To Throw only if is Held
void UScytheRecall::CanSwitch(FVector OwnerPos, FVector ScythePos, bool& CanSwitch)
{
	CanSwitch = Scythe->ScytheState == EScytheState::HELD;
}

//Reset The Parameters, update the Scythe's Roll to the prescribed one, Make the Scythe Independent
void UScytheRecall::Enable(float XDir, FVector NewTargetPoint)
{
	RollAngle = XDir == 0 ? RollAngle : RollAngle * FMath::Sign(XDir);
	ActionTimeElapsed = 0;
	TargetRoll = Scythe->GetActorRotation().Roll;
	//Continue after the ScytheThrow
	CurrentVelocity = MinVelocity;
	//Make Scythe an Independent Actor
	Scythe->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	Scythe->SetColliderCollision(ColliderCollisionChannel, ColliderCollisionEnabled, "ScytheRecall");
	Scythe->SetMeshCollision(MeshCollisionChannel, MeshCollisionEnabled, "ScytheRecall");
	
	Scythe->ScytheState = EScytheState::RECALLED;
	UE_LOG(LogTemp, Display, TEXT("Recalled Enabled"));

}
//Handle Acceleration, Lerp towards the Scythe Hand, Decelerate if close to the the Scythe 
void UScytheRecall::Update(float DeltaTime)
{
	if (Scythe->ScytheState != EScytheState::RECALLED) return;
	
	ActionTimeElapsed += DeltaTime;
	
	float DistanceToOwner = FVector::Distance(Scythe->GetActorLocation(), Scythe->ScytheHand->GetComponentLocation());
	if (DistanceToOwner <= HolsterProximity)
	{
		OnDeactivate.Broadcast();
		return;
	}
	if (DistanceToOwner <= DecelerationProximity)
	{
		AccelerationTime = 0.f;
		if (CurrentVelocity != MaxVelocity)
		{
			DecelerationTime += DeltaTime;
			CurrentVelocity = FMath::Lerp(MaxVelocity, MinVelocity,
			DecelerationCurve->GetFloatValue(DecelerationTime));
		} else
		{
			DecelerationTime = 0.f;
		}
	} else
	{
		DecelerationTime = 0.f;
		if (CurrentVelocity != MaxVelocity)
		{
			AccelerationTime += DeltaTime;
			CurrentVelocity = FMath::Lerp(MinVelocity, MaxVelocity,
			AccelerationCurve->GetFloatValue(AccelerationTime));
		} else
		{
			AccelerationTime = 0.f;
		}
	}
	
	Scythe->RotateDirection(UKismetMathLibrary::FindLookAtRotation(Scythe->GetActorLocation(),
		Scythe->ScytheHand->GetComponentLocation()));
	TargetRoll = FMath::FInterpTo(TargetRoll, RollAngle, DeltaTime, 7.5f);
	FRotator NewRotation = Scythe->GetActorRotation();
	NewRotation.Roll = TargetRoll; 
	Scythe->SetActorRotation(NewRotation);
	
	Scythe->SetActorLocation(Scythe->GetActorLocation() + Scythe->GetMovementDirection() * CurrentVelocity * DeltaTime,
		true);
	Scythe->SpinScythe(SpinSign * RotationRate * DeltaTime);
}
//Hide the Actual Scythe, Make the Scythe in Hand Visible (Done in Holster/Scythe)
void UScytheRecall::Disable()
{
	Scythe->ScytheHand->HolsterScythe();
	Scythe->ScytheState = EScytheState::HELD;
	UE_LOG(LogTemp, Display, TEXT("Recalled Disabled"));
}
//For DamageDealing
void UScytheRecall::HandleColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Scythe->ScytheState != EScytheState::RECALLED) return;
}
//For Mesh Attachment
void UScytheRecall::HandleMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Scythe->ScytheState != EScytheState::RECALLED) return;
	UE_LOG(LogTemp, Display, TEXT("Recalled Collided"));

	if (bShouldStopAtAnObstacle)
	{
		OnDeactivate.Broadcast();
	}
}