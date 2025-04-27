// Fill out your copyright notice in the Description page of Project Settings.


#include "ScytheCharge.h"

#include "AbsScytheAction.h"
#include "Pushable.h"
#include "ScytheLaunch.h"

void UScytheCharge::BeginPlay()
{
	Super::BeginPlay();
}

void UScytheCharge::AttachToAction(AScythe* NewScythe)
{
	if (ConnectedAction->Scythe->ScytheHand->Reaper->ReaperMana->CanUseAbility(OverridenActionParameters.ManaConsumption) == false) return;
	//Rewrite the struct of parameters, Add new Update Behavior & On Hit Collision Behavior
	ConnectedAction->UpdateParameters(OverridenActionParameters, false);
	
	ConnectedAction->OnMeshOverlap.RemoveDynamic(ConnectedAction, &UAbsScytheAction::HandleMeshOverlap);
	ConnectedAction->OnMeshOverlap.AddUniqueDynamic(this, &UScytheCharge::HitMesh);

	ConnectedAction->OnUpdate.RemoveDynamic(ConnectedAction, &UAbsScytheAction::Update);
	ConnectedAction->OnUpdate.AddUniqueDynamic(this, &UScytheCharge::Update);

	bShouldGoUp = false;
	CurrentPunchCount = 0;
	CurrentNonHitWindow = 0.f;
}
void UScytheCharge::DetachFromAction(AScythe* NewScythe)
{
	//Reset the struct of parameters, Have the Action Reset its Behaviors
	ConnectedAction->ResetParameters();

	ConnectedAction->OnMeshOverlap.AddUniqueDynamic(ConnectedAction, &UAbsScytheAction::HandleMeshOverlap);
	ConnectedAction->OnMeshOverlap.RemoveDynamic(this, &UScytheCharge::HitMesh);
	ConnectedAction->OnDeactivate.RemoveDynamic(this, &UScytheCharge::Deactivate);

	ConnectedAction->OnUpdate.AddUniqueDynamic(ConnectedAction, &UAbsScytheAction::Update);
	ConnectedAction->OnUpdate.RemoveDynamic(this, &UScytheCharge::Update);
}

void UScytheCharge::ActivateAbility(AScythe* NewScythe)
{
	Super::ActivateAbility(NewScythe);
	if (!ScytheLaunch) ScytheLaunch = Cast<UScytheLaunch>(ConnectedAction);

}
void UScytheCharge::Enable(float xDir, FVector TargetPoint)
{
	
}
void UScytheCharge::Disable()
{
	
}
void UScytheCharge::Charge(float ElapsedTime)
{
	if (ElapsedTime >= ActivationParameters.RequiredChargeTime)
	{
		AttachToAction(Scythe);
		GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Green, FString::Printf(TEXT("Heavy Attack Charged")));
	}
}
void UScytheCharge::Update(float DeltaTime)
{
	if (Scythe->ScytheState != EScytheState::THROWN) return;

	Scythe->ScytheHand->Reaper->ReaperMana->ReduceMana(ConnectedAction->ActionParameters.ManaConsumptionPerFrame * DeltaTime, false);
	
	FRotator NewRotation = Scythe->GetActorRotation();
	if (bShouldGoUp == false)
	{
		//Position and Rotation Adjustment
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Scythe->ScytheHand->Reaper); // Ignore self
		QueryParams.AddIgnoredActor(Scythe); // Ignore self


		FVector Start = Scythe->GetBottomPosition();
		FVector End = Start + (Scythe->GetMovementDirection().GetSafeNormal() * BottomGrazeCheckLength);
		// Perform the line trace
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,BottomGrazeTraceChannel, QueryParams);
		bool bZAxisConditionFulfilled = !(bShouldCheckZAxisOnly && HitResult.Normal.Z <= 0.f);
		if (bHit)
		{
		
			// Draw debug line (optional)
			DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, true, 10.f, 0, 1.0f);
			// Draw debug line (optional)
			DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + HitResult.Normal * 200.f, FColor::Yellow, false, 10.f, 0, 1.0f);
			// Draw debug sphere (optional)
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 32.f, 20, FColor::Purple, false, 10.f, 0, 1.0f);
			if (bZAxisConditionFulfilled)
			{
				FVector CurrentDirection = Scythe->GetMovementDirection().GetSafeNormal();

				float DotProduct = FVector::DotProduct(CurrentDirection, HitResult.Normal);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("Angle: %f"), FMath::RadiansToDegrees(FMath::Acos(DotProduct))));

				//Check if the Angle is Sharp enough
				if (DotProduct < 0.0f)
				{
					FVector SlideDirection = FVector::VectorPlaneProject(CurrentDirection, HitResult.Normal).GetSafeNormal();
					// Interpolate smoothly to avoid sudden snapping
					NewRotation.Pitch = SlideDirection.Rotation().Pitch;
				}
			
				// Ensure the bottom position is slightly raised above the impact surface
				FVector AdjustedPosition = Scythe->GetActorLocation();
		
				if (Start.Z <= HitResult.ImpactPoint.Z + GrazeClearanceLength)
				{
					AdjustedPosition.Z = HitResult.ImpactPoint.Z + GrazeClearanceLength;
					Scythe->SetActorLocation(AdjustedPosition);
				}
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.0, 0, 1.0f);
		}
	} else
	{
		NewRotation.Pitch = FMath::FInterpTo(NewRotation.Pitch, 89.f, DeltaTime, UpPitchRotationSpeed);
		CurrentNonHitWindow -= DeltaTime;
	}
	ConnectedAction->ActionTimeElapsed += DeltaTime;
	if (ConnectedAction->CurrentVelocity != ConnectedAction->ActionParameters.MaxVelocity)
	{
		ConnectedAction->AccelerationTime += DeltaTime;
		ConnectedAction->CurrentVelocity = bShouldGoUp ? UpMovementVelocity : FMath::Lerp(ConnectedAction->ActionParameters.MinVelocity, ConnectedAction->ActionParameters.MaxVelocity,
		ConnectedAction->ActionParameters.AccelerationCurve->GetFloatValue(ConnectedAction->AccelerationTime));
	} else
	{
		ConnectedAction->AccelerationTime = 0.f;
	}
	
	NewRotation.Roll = FMath::FInterpTo(NewRotation.Roll, bShouldGoUp ? 0.f :  ConnectedAction->ActionParameters.RollAngle, DeltaTime, 25.f);
	FString RotationString = NewRotation.ToString();
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Actor Rotation: %s"), *RotationString));
	bool ShouldMove = true;
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Scythe->ScytheHand->Reaper); // Ignore self
	QueryParams.AddIgnoredActor(Scythe); // Ignore self
	FVector Start = Scythe->GetActorLocation();
	FVector End = Start + (FVector::DownVector * MaxUpDistance);
	// Perform the line trace
	ShouldMove = bShouldGoUp == false || GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,BottomGrazeTraceChannel, QueryParams);

	Scythe->RotateDirection(NewRotation);
	if (ShouldMove) Scythe->SetActorLocation(Scythe->GetActorLocation() + Scythe->GetMovementDirection() * ConnectedAction->CurrentVelocity * DeltaTime,
		true);
	Scythe->SpinScythe((bShouldGoUp ? (-1) * ConnectedAction-> ActionParameters.SpinSign : ConnectedAction-> ActionParameters.SpinSign)  * ConnectedAction->ActionParameters.RotationRate * DeltaTime);
	
}

void UScytheCharge::HitMesh(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!ScytheLaunch || Scythe->ScytheState != EScytheState::THROWN || OtherActor == Scythe->ScytheHand->Reaper) return;

	IDamageable* ComponentToDamage = Cast<IDamageable>(UTypeUtil::GetFirstComponentByInterface(OtherActor, UDamageable::StaticClass()));
	IPushable* ComponentToPush = Cast<IPushable>(UTypeUtil::GetFirstComponentByInterface(OtherActor, UPushable::StaticClass()));
	
	bool bShouldScytheStop = ScytheLaunch->ActionParameters.bShouldStopAfterHit || (ScytheLaunch->ActionParameters.bShouldStopAtAnObstacle && !ComponentToDamage);
	
	if (ComponentToDamage)
	{
		float ModifiedDamage = ConnectedAction->ActionParameters.DamagePerHit;
		ComponentToDamage->ReceiveDamage(Scythe->ScytheHand->Reaper, Scythe, SweepResult, ModifiedDamage, bShouldScytheStop );
		Scythe->UpdateReaperCombo(ModifiedDamage);
	}
	if (ComponentToPush && bShouldGoUp == false && bShouldScytheStop)
	{
		bShouldGoUp = true;
		CurrentPunchCount = 1;
	} 
	if (bShouldScytheStop && (!ComponentToPush || (CurrentPunchCount > AirPunchCount && CurrentNonHitWindow <= 0.f)))
	{
		ScytheLaunch->SetStuckParent(OtherActor);
		ScytheLaunch->OnDeactivate.Broadcast();
		if (ComponentToPush) ComponentToPush -> ResetForce();
	}
	if (CurrentPunchCount > 0 && CurrentPunchCount <= AirPunchCount && CurrentNonHitWindow <= 0.f)
	{
		if (CurrentPunchCount == AirPunchCount)
		{
			ScytheLaunch->SetStuckParent(OtherActor);
		}
		CurrentNonHitWindow = NonHitWindow;
		ComponentToPush->ApplyForce(SweepResult.ImpactPoint, FVector::UpVector, CurrentPunchCount == 1 ? InitialPunchForce : AdditionalPunchForce);
		if (CurrentPunchCount == AirPunchCount)
		{
			ScytheLaunch->OnDeactivate.Broadcast();
			if (ComponentToPush) ComponentToPush -> ResetForce();
		}
		CurrentPunchCount++;
		CurrentNonHitWindow = NonHitWindow;
	} else if (ComponentToPush && !bShouldGoUp) ComponentToPush->ApplyForce(SweepResult.ImpactPoint, FVector::UpVector, AdditionalPunchForce);
}
void UScytheCharge::HitCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}