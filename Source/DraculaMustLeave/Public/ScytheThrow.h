// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbsScytheAction.h"
#include "ScytheThrow.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRACULAMUSTLEAVE_API UScytheThrow : public UAbsScytheAction
{
	GENERATED_BODY()
public:
	UScytheThrow();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Parameters")
	float TimeElapsedToRecall = .3f;
private:
	AActor* StuckParent;
	
public:
	virtual void CanSwitch(FVector OwnerPos, FVector ScythePos, UPARAM(ref) bool& CanSwitch) override;
	
	virtual void Enable(float XDir, FVector NewTargetPoint) override;
	virtual void Disable() override;
	virtual void Update(float DeltaTime) override;
	virtual void Charge(float ElapsedTime) override {return;}
	virtual void HandleColliderOverlap(UPrimitiveComponent* OverlappedComponent, 
				AActor* OtherActor, 
				UPrimitiveComponent* OtherComp, 
				int32 OtherBodyIndex, 
				bool bFromSweep, 
				const FHitResult& SweepResult) override;
	virtual void HandleMeshOverlap(UPrimitiveComponent* HitComponent,
				AActor* OtherActor,
				UPrimitiveComponent* OtherComp,
				FVector NormalImpulse,
				const FHitResult& Hit) override;
};
