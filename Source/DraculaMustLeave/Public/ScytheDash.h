// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbsScytheAbility.h"
#include "ScytheDash.generated.h"

/**
 * 
 */
UCLASS()
class DRACULAMUSTLEAVE_API UScytheDash : public UAbsScytheAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInterpolationParameters")
	FScytheActionParameters CharacterInterpolationParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInterpolationParameters")
	AReaperPawn* Reaper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall Parameters")
	float DecelerationProximity = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recall Parameters")
	float HolsterProximity = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Punch Parameters")
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Punch Parameters")
	float PunchConeDistance = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Punch Parameters")
	float PunchConeRadius = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Punch Parameters")
	FVector PunchDirection = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Punch Parameters")
	float PunchForce = 50.f;
	
protected:
	virtual void BeginPlay() override;
public:
	virtual void AttachToAction(AScythe* NewScythe) override;
	virtual void DetachFromAction(AScythe* NewScythe) override;
	virtual void ActivateAbility(AScythe* NewScythe) override;
	virtual void Enable(float xDir, FVector TargetPoint) __override;
	virtual void Disable() __override;
	virtual void Charge(float ElapsedTime) __override;
	virtual void Update(float DeltaSeconds) __override;
	virtual void HitMesh(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult) __override;
	virtual void HitCollision(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult) __override;
};
