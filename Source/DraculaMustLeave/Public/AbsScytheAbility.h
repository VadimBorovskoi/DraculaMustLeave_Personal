// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TypeUtil.h"
#include "Scythe.h"
#include "AbsScytheAbility.generated.h"

class UAbsScytheAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivationWindowOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivationWindowClose);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityAttach, AScythe*, Scythe);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityDetach, AScythe*, Scythe);

USTRUCT(BlueprintType)
struct FScytheAbilityParameters
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Parameters")
	bool bShouldCharge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Parameters")
	float RequiredChargeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Parameters")
	bool bShouldActivateOnRelease = true;
	bool bIsCharged;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation Window Parameters")
	bool bShouldHaveActivationWindow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation Window Parameters")
	float ActivationDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation Window Parameters")
	float ActivationWindowTime;
	bool bCanActivationWindowOpen;
	bool bCanActivateWithinWindow;
	float CurrentTime;
};

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UAbsScytheAbility : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbsScytheAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation Parameters")
	FScytheAbilityParameters ActivationParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Parameters")
	FScytheActionParameters OverridenActionParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	bool bIsAdditiveParams;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	bool bIsAdditiveEffects;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnActionActivate OnActivate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnActionDeactivate OnDeactivate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnCharge OnCharge;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnUpdate OnUpdate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnColliderOverlap OnColliderOverlap;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnMeshHit OnMeshOverlap;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TSubclassOf<UAbsScytheAction> ConnectedActionClass;

	UAbsScytheAction* ConnectedAction;
	AScythe* Scythe;
	bool bIsEnabled;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void MergeDelegates();
	UFUNCTION()
	void HandleActivate(float xDir, FVector TargetPoint) {OnActivate.Broadcast(xDir, TargetPoint);}
	UFUNCTION()
	void HandleDeactivate() {OnDeactivate.Broadcast();}
	UFUNCTION()
	void HandleCharge(float ElapsedTime){OnCharge.Broadcast(ElapsedTime);}
	UFUNCTION()
	void HandleUpdate(float DeltaTime) {OnUpdate.Broadcast(DeltaTime);}
	UFUNCTION()
	void HandleMeshHit( UPrimitiveComponent* HitComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit) { OnMeshOverlap.Broadcast(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);}
	UFUNCTION()
	void HandleCollisionHit(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult) { OnColliderOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);}
public:
	virtual void Activate(AScythe* NewScythe);
	UFUNCTION(Category = "Abstract")
	virtual void AttachToAction(AScythe* NewScythe) PURE_VIRTUAL(UAbsScytheAbility::AttachToAction, );
	UFUNCTION(Category = "Abstract")
	virtual void DetachFromAction(AScythe* NewScythe) PURE_VIRTUAL(UAbsScytheAbility::DetachFromAction, );
	virtual bool IsEnabled();
	
	UFUNCTION(Category = "Abstract")
	virtual void Enable(float xDir, FVector TargetPoint) PURE_VIRTUAL(UAbsScytheAbility::Enable, );
	UFUNCTION(Category = "Abstract")
	virtual void Disable() PURE_VIRTUAL(UAbsScytheAbility::Disable, );
	UFUNCTION(Category = "Abstract")
	virtual void Charge(float ElapsedTime) PURE_VIRTUAL(UAbsScytheAbility::Charge, );
	UFUNCTION(Category = "Abstract")
	virtual void Update(float DeltaSeconds) PURE_VIRTUAL(UAbsScytheAbility::Charge, );
	
	UFUNCTION(Category = "Abstract")
	virtual void HitMesh(UPrimitiveComponent* HitComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit) PURE_VIRTUAL(UAbsScytheAbility::HitMesh, );
	UFUNCTION(Category = "Abstract")
	virtual void HitCollision(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult) PURE_VIRTUAL(UAbsScytheAbility::HitCollision, );
};
