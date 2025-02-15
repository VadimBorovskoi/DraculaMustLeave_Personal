// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockAim.h"
#include "ReaperPawn.h"
#include "FOV.generated.h"

UENUM(BlueprintType)
enum class ESmoothingFunction : uint8 {
	Linear = 0 UMETA(DisplayName = "Linear"),
	Asymptotic = 1 UMETA(DisplayName = "Asymptotic"),
	EaseIn = 2 UMETA(DisplayName = "EaseIn"),
	EaseOut = 3 UMETA(DisplayName = "EaseOut"),
};

USTRUCT(BlueprintType)
struct FOffset
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransitionSmoothness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESmoothingFunction SmoothingFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAdditive;
	FOffset():Offset(0.0f), TransitionSmoothness(0.0f), SmoothingFunction(ESmoothingFunction::Asymptotic), bIsAdditive(true)
	{
	}
	// Define the equality operator
	bool operator==(const FOffset& Other) const
	{
		// Compare each member of FOffset for equality
		return Offset == Other.Offset &&
			   TransitionSmoothness == Other.TransitionSmoothness &&
			   SmoothingFunction == Other.SmoothingFunction;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UFOV : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFOV();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	FOffset DefaultFOV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	float FOVUpperLimit = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	float FOVLowerLimit = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash Parameters")
	float DashAlphaOverride = 0.75f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash Parameters")
	float MaxXValueForDash = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Offsets")
	FOffset ZoomIn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Offsets")
	FOffset DashNormal;
	
private:
	ULockAim* LockComponent;
	float CurrentFOV = 0;
	float CurrentStartingFOV = 0;
	FOffset TargetFOV;
	TArray<FOffset> Offsets;

public:	
	// Called every frame
	float GetFOV(float DeltaTime, float AdditionalMultiplier = 1.f);
	float GetInitializedFOV(ULockAim* LockAim, AReaperPawn* Reaper);
	UFUNCTION()
	void StartZoomIn(AActor* Target);
	UFUNCTION()
	void StopZoomIn(AActor* Target);
	UFUNCTION()
	void StartDashNormal(AActor* Target, float XValue);
	UFUNCTION()
	void StopDashNormal(AActor* Target, float XValue);
	void UpdateOffsets(FOffset TargetOffset, bool Add);
};
