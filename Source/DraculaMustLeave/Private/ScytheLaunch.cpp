
#include "ScytheLaunch.h"

#include "InterpolationUtil.h"
#include "Kismet/KismetMathLibrary.h"



//Can Switch if either enough Time Elapsed or Scythe is Static
void UScytheLaunch::CanSwitch(FVector OwnerPos, FVector ScythePos, bool& CanSwitch)
{
	CanSwitch = (Scythe -> ScytheState == EScytheState::THROWN && ActionTimeElapsed >= TimeElapsedToRecall)
		|| Scythe -> ScytheState == EScytheState::STATIC;
}

//Reset Parameters, Rotate the Scythe towards the Forward Direction of the Crosshair
void UScytheLaunch::Enable(float XDir, FVector NewTargetPoint)
{
	TargetPoint = NewTargetPoint;
	RollAngle =  FMath::IsNearlyEqual(XDir, 0) ? RollAngle : RollAngle * FMath::Sign(XDir);
	ActionTimeElapsed = 0;
	AccelerationTime = 0;
	DecelerationTime = 0;
	CurrentVelocity = MinVelocity;
	Scythe->SetActorLocation(Scythe->ScytheHand->GetComponentLocation());
	
	Scythe->RotateDirection(UKismetMathLibrary::FindLookAtRotation(Scythe->GetActorLocation(),
		TargetPoint));
	
	Scythe->ScytheHand->SetLocalScytheVisibility(false);

	
	Scythe->SetColliderCollision(ColliderCollisionChannel, ColliderCollisionEnabled, "ScytheThrow");
	Scythe->SetMeshCollision(MeshCollisionChannel, MeshCollisionEnabled, "ScytheThrow");
	
	Scythe->Show();
	Scythe->ScytheState = EScytheState::THROWN;
	UE_LOG(LogTemp, Display, TEXT("Thrown Enabled"));
}
//Accelerate towards the point, move there
void UScytheLaunch::Update(float DeltaTime)
{
	if (Scythe->ScytheState != EScytheState::THROWN) return;
	
	ActionTimeElapsed += DeltaTime;
	if (CurrentVelocity != MaxVelocity)
	{
		AccelerationTime += DeltaTime;
		CurrentVelocity = FMath::Lerp(MinVelocity, MaxVelocity,
	AccelerationCurve->GetFloatValue(AccelerationTime));
	} else
	{
		AccelerationTime = 0.f;
	}
	
	FRotator NewRotation = Scythe->GetActorRotation();
	NewRotation.Roll = FMath::FInterpTo(NewRotation.Roll, RollAngle, DeltaTime, 25.f);
	Scythe->SetActorRotation(NewRotation);
	Scythe->SetActorLocation(Scythe->GetActorLocation() + Scythe->GetMovementDirection() * CurrentVelocity * DeltaTime,
		true);
	Scythe->SpinScythe(SpinSign * RotationRate * DeltaTime);
}
//Attach to the overlapped actor if there is one
void UScytheLaunch::Disable()
{
	if (!StuckParent) return;
	
	Scythe->DisableCollision();
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld,true);
	Scythe->AttachToActor(StuckParent, AttachmentRules);
	Scythe->ScytheState = EScytheState::STATIC;
	UE_LOG(LogTemp, Display, TEXT("Thrown Disabled"));

}
void UScytheLaunch::HandleColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Scythe->ScytheState != EScytheState::THROWN) return;
}
//Update the new Parent Actor to the Overlapped one
void UScytheLaunch::HandleMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("Thrown Collided with: %s"), *OtherActor->GetName());
	if (Scythe->ScytheState != EScytheState::THROWN || OtherActor == Scythe->ScytheHand->Reaper) return;


	if (bShouldStopAtAnObstacle)
	{
		StuckParent = OtherActor;
		OnDeactivate.Broadcast();
	}
}

