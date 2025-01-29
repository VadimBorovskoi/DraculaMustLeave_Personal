// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbstractMovement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnable, AActor*, ActorTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisable, AActor*, ActorTarget);

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UAbstractMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbstractMovement();

	UPROPERTY(BlueprintAssignable, Category="Event Dispatchers")
	FOnEnable OnEnable;
	UPROPERTY(BlueprintAssignable, Category="Event Dispatchers")
	FOnDisable OnDisable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MaxMovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashOverrideAlpha;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DashCollision")
	TEnumAsByte<ECollisionChannel> DashCollisionChannel = ECC_Visibility;
	ACharacter* CharacterOwner;
public:
	UFUNCTION(BlueprintPure, Category = "Abstract")
	virtual void HandleMovement(float XInput, float YInput,AActor* Target,
		UPARAM(ref) float& XScale, UPARAM(ref) float& YScale, UPARAM(ref) FVector& XDirection, UPARAM(ref) FVector& YDirection) PURE_VIRTUAL(0);
	UFUNCTION(BlueprintCallable, Category = "Abstract")
	virtual void InterpDash(AActor* Target, FVector StartPosition, FVector TargetPosition, float Alpha,
		FVector& NewPosition,bool& Successful) PURE_VIRTUAL(0);
	UFUNCTION(BlueprintPure, Category = "Abstract")
	virtual void GetDashTargetPosition(AActor* Target, float XInput, float YInput, FVector& TargetPosition, bool& Successful) PURE_VIRTUAL(0);
	virtual bool IsDashBlocked(FVector PotentialPosition, float Radius, float HalfHeight, ECollisionChannel CollisionChannel );
};
