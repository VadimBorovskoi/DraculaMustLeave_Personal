// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCameraManager.h"

#include "DSP/LFO.h"
#include "GameFramework/PawnMovementComponent.h"

ACoreCameraManager::ACoreCameraManager()
{
	LockAim = CreateDefaultSubobject<ULockAim>(TEXT("LockAim"));
	Roll = CreateDefaultSubobject<UCameraBank>(TEXT("Roll"));
}
void ACoreCameraManager::BeginPlay()
{
	Super::BeginPlay();
	SetActiveAim(FreeAim);
	ActualCamera = GetOwningPlayerController()->GetPawn()->GetComponentByClass<UCameraComponent>();
}

void ACoreCameraManager::SetActiveAim(UAbstractAim* Aim, AActor* PossibleActor)
{
	if (ActiveAim) ActiveAim->OnDeactivate.Broadcast(PossibleActor);
	ActiveAim = Aim;
	ActiveAim->OnActivate.Broadcast(PossibleActor);
}
void ACoreCameraManager::ProcessInput(float Xinput, float YInput, AActor* PossibleTarget, float SensitivityRate)
{
	if (PossibleTarget)
	{
		if (ActiveAim != LockAim) SetActiveAim(LockAim, PossibleTarget);
	} else
	{
		if (ActiveAim != FreeAim) SetActiveAim(FreeAim);
	}
	InputX = Xinput;
	InputY = -YInput;
	CurrentTarget = PossibleTarget;
	GeneralSensitivity = SensitivityRate;
}
void ACoreCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
	if (!FreeAim) FreeAim = Cast<UFreeAim>(GetComponentByClass(UFreeAim::StaticClass()));

	APawn* PlayerPawn = GetOwningPlayerController()->GetPawn();
	if (!PlayerPawn) return;
	
	FRotator newRotator = GetCameraRotation();
	
	float Pitch = newRotator.Pitch;
	float Yaw = PlayerPawn->GetActorRotation().Yaw;
	float Bank = newRotator.Roll;

	if (ActiveAim)
	ActiveAim->HandleAim(InputX * YawSensitivity, InputY * PitchSensitivity, CurrentTarget, GeneralSensitivity, DeltaTime, Pitch, Yaw );
	
	if (PlayerPawn)
	Bank = Roll->GetRoll(FVector::DotProduct(PlayerPawn->GetActorRightVector(), PlayerPawn->GetVelocity()),
		PlayerPawn->GetMovementComponent()->GetMaxSpeed(), DeltaTime);
	
	FRotator result(Pitch, Yaw, Bank);
	FRotator OwnerRotator = PlayerPawn->GetActorRotation();
	OwnerRotator.Yaw = result.Yaw;
	PlayerPawn->SetActorRotation(OwnerRotator);
	
	
	if (ActualCamera)
	ActualCamera->SetRelativeRotation(FRotator(result.Pitch, GetCameraRotation().Yaw, result.Roll));

}

FRotator ACoreCameraManager::GetCameraRotation() const
{
	return ActualCamera ? ActualCamera->GetRelativeRotation() : Super::GetCameraRotation();
}






