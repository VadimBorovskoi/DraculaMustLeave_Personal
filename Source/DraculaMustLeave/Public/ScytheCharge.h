// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbsScytheAbility.h"
#include "ScytheLaunch.h"
#include "ScytheCharge.generated.h"

/**
 * 
 */
UCLASS()
class DRACULAMUSTLEAVE_API UScytheCharge : public UAbsScytheAbility
{
	GENERATED_BODY()
private:
	UScytheLaunch* ScytheLaunch;
protected:
	virtual void BeginPlay() override;
public:
	virtual void AttachToAction(AScythe* NewScythe) override;
	virtual void DetachFromAction(AScythe* NewScythe) override;
	virtual void Activate(AScythe* NewScythe) override;
	virtual void Enable(float xDir, FVector TargetPoint) __override;
	virtual void Disable() __override;
	virtual void Charge(float ElapsedTime) __override;
	virtual void Update(float DeltaSeconds) __override;
	virtual void HitMesh(UPrimitiveComponent* HitComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit) __override;
	virtual void HitCollision(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult) __override;
};
