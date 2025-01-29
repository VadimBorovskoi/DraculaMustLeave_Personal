// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMovement.h"
#include "StrafeMovement.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ReaperPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDashBegin, AActor*, ActorTarget, float, XValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDashEnd, AActor*, ActorTarget,float, XValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDashUpdate, AActor*, ActorTarget, float, Alpha);

UCLASS()
class DRACULAMUSTLEAVE_API AReaperPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AReaperPawn();

	//Dash Events
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnDashBegin OnDashBegin;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnDashEnd OnDashEnd;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Event Dispatchers")
	FOnDashUpdate OnDashUpdate;

	//Movement Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementComponents")
	TObjectPtr<UStrafeMovement> StrafeMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementAComponents")
	TObjectPtr<UBaseMovement> BaseMovement;
	UPROPERTY(BlueprintReadOnly, Category = "MovementComponents")
	UAbstractMovement* CurrentMovement;

	//Camera & Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SceneComponents")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SceneComponents")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "SceneComponents")
	USkeletalMeshComponent* FirstPersonMesh;

	//Dash Booleans
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementConditions")
	bool isDashRestrictive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementConditions")
	bool isDashing;
	
protected:
	//Input Cache
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SceneComponents")
	float CurXInput;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SceneComponents")
	float CurYInput;
public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Process")
	void ProcessInput(float InputX, float InputY, AActor* Target);
	UFUNCTION( BlueprintCallable, Category = "Input Process")
	void ResetInput();
	virtual void ProcessInput_Implementation(float InputX, float InputY, AActor* Target);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Dash")
	void ProcessDash(AActor* Target);

	UFUNCTION(BlueprintPure, Category = "Utils")
	bool CanMove();
protected:
	UFUNCTION(BlueprintCallable)
	void SwitchMovement(AActor* Target);
};
