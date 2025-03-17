// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScytheHand.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Scythe.generated.h"
UENUM(BlueprintType)
enum class EScytheState : uint8
{
	HELD     UMETA(DisplayName = "Held"),
	THROWN   UMETA(DisplayName = "Thrown"),
	STATIC   UMETA(DisplayName = "Static"),
	RECALLED UMETA(DisplayName = "Recalled")
};
class UAbsScytheAction;


UCLASS()
class DRACULAMUSTLEAVE_API AScythe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScythe();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References")
	UScytheHand* ScytheHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	EScytheState ScytheState = EScytheState::HELD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorComponents")
	TObjectPtr<UAbsScytheAction> ThrowAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorComponents")
	TObjectPtr<UAbsScytheAction> RecallAction;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SceneComponents")
	UBoxComponent* Collider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SceneComponents")
	USkeletalMeshComponent* ScytheMesh;
	//Arrow Defines Movement Direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SceneComponents")
	UArrowComponent* DirectionArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorComponents")
	UAbsScytheAction* CurrentAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	bool bIsOnPress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	bool bIsActive;
	

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="Input")
	bool ReceiveInput();
	UFUNCTION(BlueprintCallable, Category="Input")
	bool Charge(float ElapsedSeconds);
	UFUNCTION(BlueprintCallable, Category="Input")
	bool Release();

	void SetMeshCollision(FName PresetName);
	void SetColliderCollision(FName PresetName);
	UFUNCTION()
	void OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult);
	UFUNCTION()
	void OnMeshOverlap(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult);
	void DisableCollision();

	void SetOwnerHand(UScytheHand* Hand);
	//Change Pitch of the Scythe
	void SpinScythe(float AddedSpinAngle);
	//Either rotate the Arrow or itself
	void RotateDirection(FRotator NewRotation);
	//Either get Arrow's Direction or own
	FVector GetMovementDirection();
	//Is Invoked by Hand
	void Hide();
	//Is Invoked by Recall Action
	void Show();
	void UpdateReaperCombo(float DealtDamage);
private:
	void SwitchAction(UAbsScytheAction* NewAction);
};
