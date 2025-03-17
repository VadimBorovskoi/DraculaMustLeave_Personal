// Fill out your copyright notice in the Description page of Project Settings.

#include "Scythe.h"

#include "AbsScytheAction.h"
#include "ScytheRecall.h"
#include "ScytheLaunch.h"


#include "Components/BoxComponent.h"

// Sets default values
AScythe::AScythe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);

	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetCollisionObjectType(ECC_WorldDynamic);
	Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ScytheMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ScytheMesh"));
	//Or to the Root component?
	ScytheMesh->SetupAttachment(Collider);
	
	ScytheMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ScytheMesh->SetCollisionObjectType(ECC_WorldDynamic);
	ScytheMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ScytheMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	DirectionArrow->SetupAttachment(Collider);
	

	

}

// Called when the game starts or when spawned
void AScythe::BeginPlay()
{
	Super::BeginPlay();
	ThrowAction = GetComponentByClass<UScytheLaunch>();
	RecallAction = GetComponentByClass<UScytheRecall>();
	
	Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AScythe::OnColliderOverlap);
	ScytheMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &AScythe::OnMeshOverlap);
	SwitchAction(RecallAction);
}

// Called every frame
void AScythe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentAction)
	{
		CurrentAction->Update(DeltaTime);
	}
}
bool AScythe::ReceiveInput()
{
	bool CanSwitch = false;
	CurrentAction->CanSwitch(ScytheHand->GetComponentLocation(), GetActorLocation(), CanSwitch);
	UE_LOG(LogTemp, Warning, TEXT("Can Switch : %s"), CanSwitch ? TEXT("true") : TEXT("false"));

	if (!CanSwitch) return false;
	
	SwitchAction(CurrentAction == RecallAction ? ThrowAction : RecallAction);
	
	bIsOnPress = CurrentAction->IsOnPress();
	if (bIsOnPress)
	{
		CurrentAction->OnActivate.Broadcast(ScytheHand->GetReaperMovementDirection().X, ScytheHand->GetCrosshairTarget());
	}
	return true;
}
bool AScythe::Charge(float ElapsedSeconds)
{
	if (bIsOnPress || CurrentAction->IsActive()) return false;
	
	CurrentAction->OnCharge.Broadcast(ElapsedSeconds);
	return true;
}
bool AScythe::Release()
{
	if (bIsOnPress || CurrentAction->IsActive()) return false;

	CurrentAction->OnActivate.Broadcast(ScytheHand->GetReaperMovementDirection().X, ScytheHand->GetCrosshairTarget());
	return true;
}
void AScythe::SwitchAction(UAbsScytheAction* NewAction)
{
	if (CurrentAction && CurrentAction->OnDeactivate.IsBound()) CurrentAction->OnDeactivate.Broadcast();
	CurrentAction = NewAction;
}
void AScythe::SetMeshCollision(FName PresetName)
{
	ScytheMesh->SetCollisionProfileName(PresetName);
	
}
void AScythe::SetColliderCollision(FName PresetName)
{
	Collider->SetCollisionProfileName(PresetName);
	
}
void AScythe::DisableCollision()
{
	ScytheMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AScythe::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CurrentAction->OnColliderOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
void AScythe::OnMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CurrentAction->OnMeshOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
void AScythe::SpinScythe(float AddedSpinAngle)
{
	FRotator RotationDelta(AddedSpinAngle, 0.f, 0.f);
	ScytheMesh->AddLocalRotation(RotationDelta);
}
void AScythe::RotateDirection(FRotator NewRotation)
{
	//DirectionArrow->SetWorldRotation(NewRotation);
	SetActorRotation(NewRotation);
}
FVector AScythe::GetMovementDirection()
{
	//return DirectionArrow->GetForwardVector();
	return GetActorForwardVector();
}
void AScythe::Hide()
{
	ScytheMesh->SetVisibility(false);
}
void AScythe::Show()
{
	ScytheMesh->SetVisibility(true);
}

void AScythe::SetOwnerHand(UScytheHand* Hand)
{
	ScytheHand = Hand;
	if (!ThrowAction) ThrowAction = GetComponentByClass<UScytheLaunch>();
	if (!RecallAction) RecallAction = GetComponentByClass<UScytheRecall>();
}
void AScythe::UpdateReaperCombo(float DealtDamage)
{
	ScytheHand -> Reaper -> ComboHealth -> AddHealth(DealtDamage);
}







