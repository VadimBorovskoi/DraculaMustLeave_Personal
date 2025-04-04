// Fill out your copyright notice in the Description page of Project Settings.


#include "ScytheThrow.h"

#include "Damageable.h"
#include "InterpolationUtil.h"
#include "Kismet/KismetMathLibrary.h"


UScytheThrow::UScytheThrow()
{
    UE_LOG(LogTemp, Warning, TEXT("UScytheThrow Constructor Called"));
}

//Can Switch if either enough Time Elapsed or Scythe is Static
void UScytheThrow::CanSwitch(FVector OwnerPos, FVector ScythePos, bool& CanSwitch)
{
	CanSwitch = (Scythe -> ScytheState == EScytheState::THROWN && ActionTimeElapsed >= TimeElapsedToRecall)
		|| Scythe -> ScytheState == EScytheState::STATIC;
}

//Reset Parameters, Rotate the Scythe towards the Forward Direction of the Crosshair
void UScytheThrow::Enable(float XDir, FVector NewTargetPoint)
{
	TargetPoint = NewTargetPoint;
	ActionParameters.RollAngle =  FMath::IsNearlyEqual(XDir, 0) ? ActionParameters.RollAngle : ActionParameters.RollAngle * FMath::Sign(XDir);
	ActionTimeElapsed = 0;
	AccelerationTime = 0;
	DecelerationTime = 0;
	CurrentVelocity = ActionParameters.MinVelocity;
	Scythe->SetActorLocation(Scythe->ScytheHand->GetComponentLocation());
	
	Scythe->RotateDirection(UKismetMathLibrary::FindLookAtRotation(Scythe->GetActorLocation(),
		TargetPoint));
	
	Scythe->ScytheHand->SetLocalScytheVisibility(false);

	
	Scythe->SetColliderCollision("ScytheThrow");
	Scythe->SetMeshCollision( "ScytheThrow");
	
	Scythe->Show();
	Scythe->ScytheState = EScytheState::THROWN;
	UE_LOG(LogTemp, Display, TEXT("Thrown Enabled"));
}
//Accelerate towards the point, move there
void UScytheThrow::Update(float DeltaTime)
{
	if (Scythe->ScytheState != EScytheState::THROWN) return;
	
	ActionTimeElapsed += DeltaTime;
	if (CurrentVelocity != ActionParameters.MaxVelocity)
	{
		AccelerationTime += DeltaTime;
		CurrentVelocity = FMath::Lerp(ActionParameters.MinVelocity, ActionParameters.MaxVelocity,
	ActionParameters.AccelerationCurve->GetFloatValue(AccelerationTime));
	} else
	{
		AccelerationTime = 0.f;
	}
	
	FRotator NewRotation = Scythe->GetActorRotation();
	NewRotation.Roll = FMath::FInterpTo(NewRotation.Roll, ActionParameters.RollAngle, DeltaTime, 25.f);
	Scythe->SetActorRotation(NewRotation);
	Scythe->SetActorLocation(Scythe->GetActorLocation() + Scythe->GetMovementDirection() * CurrentVelocity * DeltaTime,
		true);
	Scythe->SpinScythe(ActionParameters.SpinSign * ActionParameters.RotationRate * DeltaTime);
}
//Attach to the overlapped actor if there is one
void UScytheThrow::Disable()
{
	if (!StuckParent) return;
	
	Scythe->DisableCollision();
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld,true);
	Scythe->AttachToActor(StuckParent, AttachmentRules);
	Scythe->ScytheState = EScytheState::STATIC;
	UE_LOG(LogTemp, Display, TEXT("Thrown Disabled"));

}
void UScytheThrow::HandleColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Scythe->ScytheState != EScytheState::THROWN) return;
}
//Update the new Parent Actor to the Overlapped one
void UScytheThrow::HandleMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("Thrown Collided with: %s"), *OtherActor->GetName());
	if (Scythe->ScytheState != EScytheState::THROWN || OtherActor == Scythe->ScytheHand->Reaper) return;

	IDamageable* ActorToDamage = Cast<IDamageable>(OtherActor);

	bool bShouldScytheStop = ActionParameters.bShouldStopAtAnObstacle;
	if (ActorToDamage)
	{
		float ModifiedDamage = ActionParameters.DamagePerHit;
		ActorToDamage->ReceiveDamage(Scythe->ScytheHand->Reaper, Scythe, ModifiedDamage, bShouldScytheStop );
		Scythe->UpdateReaperCombo(ModifiedDamage);
	}
	if (bShouldScytheStop)
	{
		StuckParent = OtherActor;
		OnDeactivate.Broadcast();
	}
}



