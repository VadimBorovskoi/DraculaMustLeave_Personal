
#pragma once

#include "CoreMinimal.h"
#include "AbsScytheAbility.h"
#include "Scythe.h"
#include "TypeUtil.h"
#include "Components/ActorComponent.h"
#include "AbsScytheAction.generated.h"

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UAbsScytheAction : public UActorComponent
{
	
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbsScytheAction();
	AScythe* Scythe;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	FScytheActionParameters ActionParameters;
	
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
	FOnMeshOverlap OnMeshOverlap;

	TArray<UAbsScytheAbility*> AbilityArray;

	FVector TargetPoint;
	float ActionTimeElapsed;
	float CurrentVelocity;
	float DecelerationTime = 0.f;
	float AccelerationTime = 0.f;

protected:
	FScytheActionParameters DefaultActionParameters;
	TArray<FScytheActionParameters> ParametersPool;
protected:
		// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure, Category = "Abstract")
	virtual void CanSwitch(FVector OwnerPos, FVector ScythePos, UPARAM(ref) bool& CanSwitch) PURE_VIRTUAL(0);
	//Check if Charged Enough?
	UFUNCTION(BlueprintPure)
	virtual bool IsOnPress() {return OnCharge.IsBound() == false;}
	UFUNCTION()
	virtual void UpdateParameters(FScytheActionParameters NewParams, bool bIsAdditive);
	UFUNCTION()
	virtual void ResetParameters()
	{
		if (ActionParameters == DefaultActionParameters && ParametersPool.Num() == 1.f) return;
		ActionParameters = DefaultActionParameters;
		ParametersPool.Empty();
		ParametersPool.Add(ActionParameters);
	}
	//Check if any ability in the pool Should Activate On Release
	UFUNCTION(BlueprintPure)
	virtual bool ShouldAutoActivateWhenCharged() {
		for (int32 i = 0; i < AbilityArray.Num(); i++)
		{
			if (AbilityArray[i]->ActivationParameters.bShouldActivateOnRelease) return true;
		}
		return false;
	}
	
	UFUNCTION()
	virtual  void Enable(float XDir, FVector NewTargetPoint) PURE_VIRTUAL(0);
	UFUNCTION()
	virtual void Disable() PURE_VIRTUAL(0);
	UFUNCTION()
	virtual void Update(float DeltaTime)PURE_VIRTUAL(0);
	UFUNCTION()
	virtual void Charge(float ElapsedTime) PURE_VIRTUAL(0);
	UFUNCTION()
	virtual void HandleColliderOverlap(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult) PURE_VIRTUAL(0);
	UFUNCTION()
	virtual void HandleMeshOverlap(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult) PURE_VIRTUAL(0);
};
