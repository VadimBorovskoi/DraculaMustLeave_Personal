// Fill out your copyright notice in the Description page of Project Settings.


#include "ReaperPawn.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AReaperPawn::AReaperPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm= CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300;
	SpringArm->bUsePawnControlRotation=false;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation=false;

	FirstPersonMesh=CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	FirstPersonMesh->SetupAttachment(Camera);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;
	
}
void AReaperPawn::BeginPlay()
{
	Super::BeginPlay();
	BaseMovement = GetComponentByClass<UBaseMovement>();
	StrafeMovement = GetComponentByClass<UStrafeMovement>();
	ComboHealth = GetComponentByClass<UHealthCombo>();
	CurrentMovement = BaseMovement;
}
void AReaperPawn::ProcessInput_Implementation(float InputX, float InputY, AActor* Target)
{
	CurXInput = InputX;
	CurYInput = InputY;
	if (!CanMove()) return;
	
 	SwitchMovement(Target);
	float XScale = 0;
	float YScale = 0;
	FVector XDirection = FVector(InputX, 0, 0);
	FVector YDirection = FVector(InputY, 0, 0);
	CurrentMovement->HandleMovement(InputX, InputY, Target,
		XScale, YScale, XDirection, YDirection );
	
	AddMovementInput(XDirection, XScale, false);
	AddMovementInput(YDirection, YScale, false);
}
void AReaperPawn::ResetInput()
{
	CurXInput = 0;
	CurYInput = 0;
}

void AReaperPawn::SwitchMovement(AActor* Target)
{
	if (Target == nullptr || Target->IsValidLowLevel() == false)
	{
		if (CurrentMovement != BaseMovement)
		{
			CurrentMovement->OnDisable.Broadcast(Target);
			CurrentMovement = BaseMovement;
			CurrentMovement->OnEnable.Broadcast(Target);
		}
	} else
	{
		if (CurrentMovement != StrafeMovement)
		{
			CurrentMovement->OnDisable.Broadcast(Target);
			CurrentMovement = StrafeMovement;
			CurrentMovement->OnEnable.Broadcast(Target);
		}
	}
	GetCharacterMovement()->MaxWalkSpeed = CurrentMovement->MaxMovementSpeed;
}
bool AReaperPawn::CanMove()
{
	return isDashing == false || isDashRestrictive == false;
}





