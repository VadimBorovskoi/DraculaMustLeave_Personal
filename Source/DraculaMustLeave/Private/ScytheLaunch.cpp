
#include "ScytheLaunch.h"

#include "Damageable.h"
#include "InterpolationUtil.h"
#include "TypeUtil.h"
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
	if (Scythe-> ScytheState == EScytheState::THROWN) return;
	
	FScytheActionParameters NewParams;
	for (int32 i = 0; i < ParametersPool.Num(); i++)
	{
		if (i == 0)
		{
			NewParams += ActionParameters;
		} else
		{
			NewParams += ParametersPool[i];
		}
	}
	ActionParameters = NewParams;
	
	for (int32 i = 0; i < AbilityArray.Num(); i++)
	{
		
		if (AbilityArray[i]->ActivationParameters.bShouldHaveActivationWindow)
		{
			AbilityArray[i]->ActivationParameters.bCanActivateWithinWindow = false;
			AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = false;
		}
	}
	
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
	Scythe->SetMeshCollision("ScytheThrow");
	
	Scythe->Show();
	Scythe->ScytheState = EScytheState::THROWN;
	UE_LOG(LogTemp, Display, TEXT("Thrown Enabled"));
}
//Accelerate towards the point, move there
void UScytheLaunch::Update(float DeltaTime)
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
void UScytheLaunch::Disable()
{
	for (int32 i = 0; i < AbilityArray.Num(); i++)
	{
		AbilityArray[i]->DetachFromAction(Scythe);
	}
	
	if (StuckParent == nullptr) return;
	
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
	UE_LOG(LogTemp, Display, TEXT("Launched Collided with: %s"), *OtherActor->GetName());
	if (Scythe->ScytheState != EScytheState::THROWN || OtherActor == Scythe->ScytheHand->Reaper) return;

	IDamageable* ComponentToDamage = Cast<IDamageable>(UTypeUtil::GetFirstComponentByInterface(OtherActor, UDamageable::StaticClass()));

	bool bShouldScytheStop = ActionParameters.bShouldStopAfterHit || (ActionParameters.bShouldStopAtAnObstacle && !ComponentToDamage);
	
	if (ComponentToDamage)
	{
		float ModifiedDamage = ActionParameters.DamagePerHit;
		ComponentToDamage->ReceiveDamage(Scythe->ScytheHand->Reaper, Scythe, ModifiedDamage, bShouldScytheStop );
		Scythe->UpdateReaperCombo(ModifiedDamage);
	}
	if (bShouldScytheStop)
	{
		StuckParent = OtherActor;
		OnDeactivate.Broadcast();
	}
}

