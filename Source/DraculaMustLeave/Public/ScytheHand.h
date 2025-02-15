// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReaperPawn.h"
#include "Components/SceneComponent.h"
#include "ScytheHand.generated.h"

class AScythe;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRACULAMUSTLEAVE_API UScytheHand : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScytheHand();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* ScytheMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TSubclassOf<AScythe> ScytheType;
	UPROPERTY(BlueprintReadOnly, Category="Scythe")
	AScythe* Scythe;
	AReaperPawn* Reaper;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="ScytheManagement")
	void HolsterScythe();
	UFUNCTION(BlueprintCallable, Category="Input")
	void ScythePress();
	UFUNCTION(BlueprintCallable, Category="Input")
	void ScytheHold(float ElapsedSeconds);
	UFUNCTION(BlueprintCallable, Category="Input")
	void ScytheRelease();
	void SetLocalScytheVisibility(bool IsVisible) const;
	FVector GetCrosshairTarget() const;
	FVector GetReaperMovementDirection() const;
};
