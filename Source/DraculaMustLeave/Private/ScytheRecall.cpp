// Fill out your copyright notice in the Description page of Project Settings.


#include "ScytheRecall.h"

#include "Damageable.h"
#include "InterpolationUtil.h"
#include "TypeUtil.h"
#include "Kismet/KismetMathLibrary.h"

//Can Switch To Throw only if is Held
void UScytheRecall::CanSwitch(FVector OwnerPos, FVector ScythePos, bool& CanSwitch)
{
	
	CanSwitch = Scythe->ScytheState == EScytheState::HELD;
	for (int32 i = 0; i < Scythe->ThrowAction->AbilityArray.Num(); i++)
	{
		if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bShouldHaveActivationWindow)
		{
			 if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen)
			 {
			 	Scythe->ThrowAction->AbilityArray[i]->AttachToAction(Scythe);
			 	Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = false;
			 	Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivateWithinWindow = false;
			 	CanSwitch = true;
			 	return;
			 }
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Clicked Outside of the Window")));

			Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = false;
			Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivateWithinWindow = false;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,  FString::Printf(TEXT("Is Window Open: %s"), Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen ? TEXT("true") : TEXT("false")));

		}
	}
}

//Reset The Parameters, update the Scythe's Roll to the prescribed one, Make the Scythe Independent
void UScytheRecall::Enable(float XDir, FVector NewTargetPoint)
{
	if (Scythe->ScytheState == EScytheState::RECALLED) return;
	
	FScytheActionParameters NewParams;
	for (int32 i = 0; i < ParametersPool.Num(); i++)
	{
		NewParams += ParametersPool[i];
	}
	ActionParameters = NewParams;
	
	ActionParameters.RollAngle = XDir == 0 ? ActionParameters.RollAngle : ActionParameters.RollAngle * FMath::Sign(XDir);
	ActionTimeElapsed = 0;
	TargetRoll = Scythe->GetActorRotation().Roll;
	//Continue after the ScytheThrow
	CurrentVelocity = ActionParameters.MinVelocity;
	//Make Scythe an Independent Actor
	Scythe->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	Scythe->SetColliderCollision("ScytheRecall");
	Scythe->SetMeshCollision("ScytheRecall");
	
	Scythe->ScytheState = EScytheState::RECALLED;
	UE_LOG(LogTemp, Display, TEXT("Recalled Enabled"));
	
	//Reset the Activation window for the Combo Ability of Launch Action
	for (int32 i = 0; i < Scythe->ThrowAction->AbilityArray.Num(); i++)
	{
		if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bShouldHaveActivationWindow)
		{
			Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivateWithinWindow = true;
			Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = false;
			UE_LOG(LogTemp, Warning, TEXT("Combo Window Set Up"));

		}
	}
	
	Scythe->ScytheHand->Reaper->ReaperMana->ReduceMana(ActionParameters.ManaConsumption, true);
}
//Handle Acceleration, Lerp towards the Scythe Hand, Decelerate if close to the Scythe 
void UScytheRecall::Update(float DeltaTime)
{
	//Update the Combo Windows' Timer
	for (int32 i = 0; i < Scythe->ThrowAction->AbilityArray.Num(); i++)
	{

		if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bShouldHaveActivationWindow)
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White,  FString::Printf(TEXT("Is Window Open: %s"), Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivateWithinWindow ? TEXT("true") : TEXT("false")));

			if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen == false) continue;
			Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime -= DeltaTime;
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Time: %f"), Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime));

			if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime <= 0.0f)
			{
				Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = false;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Combo Window Closed")));

			}
		}
	}
	
	if (Scythe->ScytheState != EScytheState::RECALLED) return;
	
	Scythe->ScytheHand->Reaper->ReaperMana->ReduceMana(ActionParameters.ManaConsumptionPerFrame, false);

	ActionTimeElapsed += DeltaTime;
	
	float DistanceToOwner = FVector::Distance(Scythe->GetActorLocation(), Scythe->ScytheHand->GetComponentLocation());

	//Managing the Combo Abilities for Launch action
	for (int32 i = 0; i < Scythe->ThrowAction->AbilityArray.Num(); i++)
	{
		if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bShouldHaveActivationWindow)
		{
			if ( DistanceToOwner <= Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.ActivationDistance
				&& 	Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivateWithinWindow
				&& Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen == false)
			{
				Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = true;
				Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime = Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.ActivationWindowTime;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::Printf(TEXT("Combo Window Open")));

			}
			Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime -= DeltaTime;
			if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime <= 0.0f)
			{
				Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = false;
			}
		}
	}
	
	if (DistanceToOwner <= HolsterProximity)
	{
		OnDeactivate.Broadcast();
		return;
	}
	if (DistanceToOwner <= DecelerationProximity)
	{
		AccelerationTime = 0.f;
		if (CurrentVelocity != ActionParameters.MaxVelocity)
		{
			DecelerationTime += DeltaTime;
			CurrentVelocity = FMath::Lerp(ActionParameters.MaxVelocity, ActionParameters.MinVelocity,
			ActionParameters.DecelerationCurve->GetFloatValue(DecelerationTime));
		} else
		{
			DecelerationTime = 0.f;
		}
	} else
	{
		DecelerationTime = 0.f;
		if (CurrentVelocity != ActionParameters.MaxVelocity)
		{
			AccelerationTime += DeltaTime;
			CurrentVelocity = FMath::Lerp(ActionParameters.MinVelocity, ActionParameters.MaxVelocity,
			ActionParameters.AccelerationCurve->GetFloatValue(AccelerationTime));
		} else
		{
			AccelerationTime = 0.f;
		}
	}
	
	Scythe->RotateDirection(UKismetMathLibrary::FindLookAtRotation(Scythe->GetActorLocation(),
		Scythe->ScytheHand->GetComponentLocation()));
	TargetRoll = FMath::FInterpTo(TargetRoll, ActionParameters.RollAngle, DeltaTime, 7.5f);
	FRotator NewRotation = Scythe->GetActorRotation();
	NewRotation.Roll = TargetRoll; 
	Scythe->SetActorRotation(NewRotation);
	
	Scythe->SetActorLocation(Scythe->GetActorLocation() + Scythe->GetMovementDirection() * CurrentVelocity * DeltaTime,
		true);
	Scythe->SpinScythe(ActionParameters.SpinSign * ActionParameters.RotationRate * DeltaTime);
}

//Hide the Actual Scythe, Make the Scythe in Hand Visible (Done in Holster/Scythe)
void UScytheRecall::Disable()
{
	for (int32 i = 0; i < AbilityArray.Num(); i++)
	{
		AbilityArray[i]->DetachFromAction(Scythe);
	}
	
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
	if (Scythe->ScytheState != EScytheState::RECALLED || OtherActor == Scythe->ScytheHand->Reaper) return;
	UE_LOG(LogTemp, Display, TEXT("Recalled Collided"));

	IDamageable* ComponentToDamage = Cast<IDamageable>(UTypeUtil::GetFirstComponentByInterface(OtherActor, UDamageable::StaticClass()));

	bool bShouldScytheStop = ActionParameters.bShouldStopAtAnObstacle;
	
	if (ComponentToDamage)
	{
		float ModifiedDamage = ActionParameters.DamagePerHit;
		ComponentToDamage->ReceiveDamage(Scythe->ScytheHand->Reaper, Scythe, ModifiedDamage, bShouldScytheStop );
		Scythe->UpdateReaperCombo(ModifiedDamage);
	}
	if (ActionParameters.bShouldStopAtAnObstacle)
	{
		OnDeactivate.Broadcast();
	}
}