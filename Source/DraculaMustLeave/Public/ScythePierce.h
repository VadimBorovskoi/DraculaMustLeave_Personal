// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbsScytheAbility.h"
#include "ScytheRecall.h"
#include "ScythePierce.generated.h"

/**
 * 
 */
UCLASS()
class DRACULAMUSTLEAVE_API UScythePierce : public UAbsScytheAbility
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void MergeDelegates() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TSubclassOf<UAbsScytheAction> SecondaryActionClass;

	UAbsScytheAction* SecondaryAction;
	UScytheRecall ScytheRecall;
public:
	virtual void Activate(AScythe* NewScythe) override;
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
protected:
	UFUNCTION()
	virtual void DetectActivationWindow(float DeltaSeconds);
	UFUNCTION()
	virtual void CheckCorrectTiming();
};
