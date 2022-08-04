// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword_FS.generated.h"

UCLASS()
class T_CPP_API ASword_FS : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* CreationPoint;

	UPROPERTY(EditAnywhere)
		FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Sword;

	UPROPERTY()
		FTimerHandle LocationTimerHandle;

	UPROPERTY()
		FTimerHandle RotationTimerHandle;

	UPROPERTY()
		FTimerHandle DelayTimerHandle;
	
	UFUNCTION()
		void UpdateLocation();

	UFUNCTION()
		void UpdateRotation();

	UFUNCTION()
		void ClearTimer();

	UFUNCTION()
		void BeginOverlap
		(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

public:	
	// Sets default values for this actor's properties
	ASword_FS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
