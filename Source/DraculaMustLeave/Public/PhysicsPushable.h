// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pushable.h"
#include "Components/ActorComponent.h"
#include "PhysicsPushable.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UPhysicsPushable : public UActorComponent, public IPushable
{
	GENERATED_BODY()

public:	
	UPhysicsPushable();
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnPush OnPush;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Params")
	TEnumAsByte<ECollisionChannel> SurfaceDetectionChannel = ECC_Visibility;
	UPrimitiveComponent* RootComponent;
	float InitialCachedHeight;
	bool bShouldResetPhysics;
	float GravityDelay = 1.f;
	float CurrentGravityDelay = 0.f;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	virtual void ApplyForce(const FVector ImpactPosition, const FVector Direction, const float Force) override;
	virtual void UpdateOwner(float DeltaTime) override;
	virtual void ResetForce() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
