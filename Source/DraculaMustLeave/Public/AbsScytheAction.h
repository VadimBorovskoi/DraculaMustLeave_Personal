
#pragma once

#include "CoreMinimal.h"
#include "Scythe.h"
#include "Components/ActorComponent.h"
#include "AbsScytheAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionActivate, float, XDir, FVector, TargetPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionDeactivate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharge, float, ElapsedTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnColliderOverlap,UPrimitiveComponent*, OverlappedComponent, 
			AActor*, OtherActor, 
			UPrimitiveComponent*, OtherComp, 
			int32, OtherBodyIndex, 
			bool, bFromSweep, 
			const FHitResult&, SweepResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnMeshOverlap,UPrimitiveComponent*, OverlappedComponent, 
			AActor*, OtherActor, 
			UPrimitiveComponent*, OtherComp, 
			int32, OtherBodyIndex, 
			bool, bFromSweep, 
			const FHitResult&, SweepResult);


UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UAbsScytheAction : public UActorComponent
{
	
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbsScytheAction();
	AScythe* Scythe;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Parameters")
	TEnumAsByte<ECollisionChannel> ColliderCollisionChannel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Parameters")
	TEnumAsByte<ECollisionChannel> MeshCollisionChannel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Parameters")
	TEnumAsByte<ECollisionEnabled::Type> MeshCollisionEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Parameters")
	TEnumAsByte<ECollisionEnabled::Type> ColliderCollisionEnabled;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Parameters")
	bool bShouldStopAtAnObstacle = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Parameters")
	bool bShouldStopAfterHit = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Parameters")
	float MaxVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Parameters")
	float MinVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Parameters")
	UCurveFloat* AccelerationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Parameters")
	UCurveFloat* DecelerationCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Parameters")
	float RotationRate;
	//int RollSign;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Parameters")
	float RollAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Parameters")
	int SpinSign;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Parameters")
	float ManaConsumption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Parameters")
	float ManaConsumptionPerFrame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Parameters")
	float DamagePerHit;
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

	FVector TargetPoint;
	float ActionTimeElapsed;
	float CurrentVelocity;
	float DecelerationTime = 0.f;
	float AccelerationTime = 0.f;

	
protected:
		// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure, Category = "Abstract")
	virtual void CanSwitch(FVector OwnerPos, FVector ScythePos, UPARAM(ref) bool& CanSwitch) PURE_VIRTUAL(0);
	//Check if Charged Enough?
	UFUNCTION(BlueprintPure, Category = "Abstract")
	virtual void CanActivate(UPARAM(ref) bool& CanActivate) PURE_VIRTUAL(0);
	UFUNCTION(BlueprintPure)
	virtual bool IsOnPress() {return true;};

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
