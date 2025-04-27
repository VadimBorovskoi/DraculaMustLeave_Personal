// Fill out your copyright notice in the Description page of Project Settings.


#include "ScytheDash.h"
#include "AbsScytheAction.h"
#include "PhysicsPushable.h"
#include "Kismet/KismetMathLibrary.h"
#include "TraceUtil.h"

void UScytheDash::BeginPlay()
{
	Super::BeginPlay();
}


void UScytheDash::AttachToAction(AScythe* NewScythe)
{
	if (ConnectedAction->Scythe->ScytheHand->Reaper->ReaperMana->CanUseAbility(OverridenActionParameters.ManaConsumption) == false) return;

	if (!Reaper) Reaper = Scythe->ScytheHand->Reaper;

	ConnectedAction->UpdateParameters(OverridenActionParameters, false);
	ConnectedAction->OnMeshOverlap.RemoveDynamic(ConnectedAction, &UAbsScytheAction::HandleMeshOverlap);
	ConnectedAction->OnMeshOverlap.AddUniqueDynamic(this, &UScytheDash::HitMesh);

	ConnectedAction->OnUpdate.RemoveDynamic(ConnectedAction, &UAbsScytheAction::Update);
	ConnectedAction->OnUpdate.AddUniqueDynamic(this, &UScytheDash::Update);
	
	ConnectedAction->OnActivate.RemoveDynamic(ConnectedAction, &UAbsScytheAction::Enable);
    ConnectedAction->OnActivate.AddUniqueDynamic(this, &UScytheDash::Enable);
	ConnectedAction->OnDeactivate.AddUniqueDynamic(this, &UScytheDash::Disable);
}

void UScytheDash::DetachFromAction(AScythe* NewScythe)
{
	ConnectedAction->ResetParameters();

	ConnectedAction->OnMeshOverlap.AddUniqueDynamic(ConnectedAction, &UAbsScytheAction::HandleMeshOverlap);
	ConnectedAction->OnMeshOverlap.RemoveDynamic(this, &UScytheDash::HitMesh);
	ConnectedAction->OnDeactivate.RemoveDynamic(this, &UScytheDash::Disable);

	ConnectedAction->OnUpdate.AddUniqueDynamic(ConnectedAction, &UAbsScytheAction::Update);
	ConnectedAction->OnUpdate.RemoveDynamic(this, &UScytheDash::Update);

	ConnectedAction->OnActivate.AddUniqueDynamic(ConnectedAction, &UAbsScytheAction::Enable);
	ConnectedAction->OnActivate.RemoveDynamic(this, &UScytheDash::Enable);
}
void UScytheDash::ActivateAbility(AScythe* NewScythe)
{
	Super::ActivateAbility(NewScythe);
}

void UScytheDash::Enable(float xDir, FVector TargetPoint)
{
	if (Scythe->ScytheState == EScytheState::RECALLED) return;
	ConnectedAction->ActionTimeElapsed = 0;
	//Continue after the ScytheThrow
	ConnectedAction->CurrentVelocity = ConnectedAction->ActionParameters.MinVelocity;
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
	
	if (ConnectedAction->ActionParameters.ManaConsumption > 0.0f)
	{
		Scythe->ScytheHand->Reaper->ReaperMana->ReduceMana(ConnectedAction->ActionParameters.ManaConsumption, true);
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Dash Attack Enabled")));

	Reaper->AlterPhysicsAndInput(true, true, true);
	Reaper->OnReaperColliderOverlap.AddUniqueDynamic(this, &UScytheDash::HitCollision);
}

void UScytheDash::Disable()
{
	if (Scythe->ScytheState == EScytheState::RECALLED) return;

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Dash Attack Disabled")));
	Reaper->AlterPhysicsAndInput(false, false, false);
	Reaper->OnReaperColliderOverlap.RemoveDynamic(this, &UScytheDash::HitCollision);

	TArray<FHitResult> ActorsToPunch = UTraceUtil::ConeTraceByChannel(this, Reaper->GetActorLocation(), Reaper->GetActorLocation() + Reaper->GetActorForwardVector() * PunchConeDistance,
		PunchConeRadius, TraceTypeQuery, true, {Reaper}, EDrawDebugTrace::ForDuration , true, EConeDebugShape::Cone, FColor::Blue, FColor::Red, 10.f );
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Actors Found %d"), ActorsToPunch.Num()));
	for (int32 i = 0; i < ActorsToPunch.Num(); i++)
	{

		UPhysicsPushable* PushableComponent = ActorsToPunch[i].GetActor()->GetComponentByClass<UPhysicsPushable>();
		if (PushableComponent)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, FString::Printf(TEXT("Pushing")));

			FVector ActualDirection = ((ActorsToPunch[i].ImpactPoint - Reaper->GetActorLocation()) + PunchDirection).GetSafeNormal();
			PushableComponent->ApplyForce(ActorsToPunch[i].ImpactPoint, ActualDirection , PunchForce);
		}
	}
	//Remove from Player Hit Collision Event
}

void UScytheDash::Charge(float ElapsedTime)
{
	if (Scythe->ScytheState != EScytheState::STATIC)
		Scythe->SpinScythe(ConnectedAction->ActionParameters.SpinSign * ConnectedAction->ActionParameters.RotationRate
			* GetWorld()->GetDeltaSeconds());
	if (ElapsedTime >= ActivationParameters.RequiredChargeTime)
	{
		AttachToAction(Scythe);
		GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Green, FString::Printf(TEXT("Dash Attack Charged")));
	}
}

void UScytheDash::Update(float DeltaSeconds)
{
	if (Scythe->ScytheState != EScytheState::RECALLED)
	{
		if (Scythe->ScytheState != EScytheState::STATIC)
		Scythe->SpinScythe(ConnectedAction->ActionParameters.SpinSign * ConnectedAction->ActionParameters.RotationRate
			* DeltaSeconds);
		return;
	} 
	GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Blue, FString::Printf(TEXT("Dash Attack Taking Place")));

	//Update the Combo Windows' Timer
	for (int32 i = 0; i < Scythe->ThrowAction->AbilityArray.Num(); i++)
	{

		if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bShouldHaveActivationWindow)
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::White,  FString::Printf(TEXT("Is Window Open: %s"), Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivateWithinWindow ? TEXT("true") : TEXT("false")));

			if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen == false) continue;
			Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime -= DeltaSeconds;
			GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Yellow, FString::Printf(TEXT("Time: %f"), Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime));

			if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime <= 0.0f)
			{
				Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = false;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Combo Window Closed")));

			}
		}
	}
	
	if (Scythe->ScytheState != EScytheState::RECALLED) return;
	
	if (ConnectedAction->ActionParameters.ManaConsumptionPerFrame > 0.0f)
	{
		Scythe->ScytheHand->Reaper->ReaperMana->ReduceMana(ConnectedAction->ActionParameters.ManaConsumptionPerFrame * DeltaSeconds, false);
	}
	ConnectedAction->ActionTimeElapsed += DeltaSeconds;
	
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
			Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime -= DeltaSeconds;
			if (Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.CurrentTime <= 0.0f)
			{
				Scythe->ThrowAction->AbilityArray[i]->ActivationParameters.bCanActivationWindowOpen = false;
			}
		}
	}
	
	if (DistanceToOwner <= HolsterProximity)
	{
		ConnectedAction->OnDeactivate.Broadcast();
		return;
	}
	if (DistanceToOwner <= DecelerationProximity)
	{
		ConnectedAction->AccelerationTime = 0.f;
		if (ConnectedAction->CurrentVelocity != ConnectedAction->ActionParameters.MaxVelocity)
		{
			ConnectedAction->DecelerationTime += DeltaSeconds;
			ConnectedAction->CurrentVelocity = FMath::Lerp(ConnectedAction->ActionParameters.MaxVelocity, ConnectedAction->ActionParameters.MinVelocity,
			ConnectedAction->ActionParameters.DecelerationCurve->GetFloatValue(ConnectedAction->DecelerationTime));
		} else
		{
			ConnectedAction->DecelerationTime = 0.f;
		}
	} else
	{
		ConnectedAction->DecelerationTime = 0.f;
		if (ConnectedAction->CurrentVelocity != ConnectedAction->ActionParameters.MaxVelocity)
		{
			ConnectedAction->AccelerationTime += DeltaSeconds;
			ConnectedAction->CurrentVelocity = FMath::Lerp(ConnectedAction->ActionParameters.MinVelocity, ConnectedAction->ActionParameters.MaxVelocity,
			ConnectedAction->ActionParameters.AccelerationCurve->GetFloatValue(ConnectedAction->AccelerationTime));
		} else
		{
			ConnectedAction->AccelerationTime = 0.f;
		}
	}
	
	
	Reaper->SetActorLocation(Reaper->GetActorLocation() + UKismetMathLibrary::FindLookAtRotation(Reaper->GetActorLocation(),
		Scythe->GetActorLocation()).Vector() * ConnectedAction->CurrentVelocity * DeltaSeconds,
		true);
}

void UScytheDash::HitMesh(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void UScytheDash::HitCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
