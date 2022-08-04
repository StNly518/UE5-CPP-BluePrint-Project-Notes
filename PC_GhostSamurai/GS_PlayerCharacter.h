// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../CharacterInterface.h"
#include <T_CPP/MotionStats_Enum.h>

#include "NiagaraSystem.h"
#include "GS_PlayerCharacter.generated.h"


UCLASS()
class T_CPP_API AGS_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


public:
	// Sets default values for this character's properties
	AGS_PlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float MoveForwardValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float MoveRightValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool ChangeToApose = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool ChangeComplete = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool ChangeAttackCombo = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool SaveAttack = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsAttack = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int AttackCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString AttackName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float AttackDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool DoDamage = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsBlockHit = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        bool CanDodge = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsDodging = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool LockedOn = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* LockedActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* NearestActor;
	UFUNCTION(BlueprintCallable)
		TArray< FHitResult > GetNearestEnemy();
	UFUNCTION(BlueprintCallable)
		void ChangeIndex(bool BeginDis,int index,int arrayLength);
	
	UFUNCTION(BlueprintCallable)
		void TraceEvent(USceneComponent* Actor,bool &IsHit,FHitResult &Hit);

	UFUNCTION(BlueprintCallable)
		AActor* DetectEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array)
		TArray<AActor*> ActorsToIgnore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Niagara)
		UNiagaraSystem* Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack00;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack02;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack11;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack13;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack14;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* Attack15;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool AbleToAim = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool AbleToBlock = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsBroken = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool Counter = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DamageCounts = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStates)
		float PS_Def;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStates)
		float PS_Nax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStates)
		float PS_Cri;
	
	UPROPERTY()
		class UUserWidget* HealthHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
		TSubclassOf<class UUserWidget> HealthHUDClass;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	)override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void ChangeButtle();

	void AttackLeft();
	void AttackEnd();

	void AimingorBlock();
	UFUNCTION(BlueprintCallable)
		void CameraMovement(float Length,FVector Location);

	void EndofRight();

	void RainShoots();
	void RainShootEnd();

	void BurstShoots();
	void BurstShootEnd();

	void FastShoots();
	void FastShootEnd();

	void Dodge();
	
	UFUNCTION(BlueprintCallable)
		void UpdateDirection(float ForwardValue,float RightValue);

	UFUNCTION(BlueprintCallable)
		void ComboAttackSave();

	UFUNCTION(BlueprintCallable)
		void ResetCombo();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float InputZ;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool EnableRun = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool RainShoot = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool FastShoot = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool BurstShoot = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool PowerOn = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool SlideShoot = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool ShootTap = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsDodge = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DirectionValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ACharacter* Player;
	
	UPROPERTY()
		FTimerHandle DirectionTimerHandle;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<EWeapon> Weapon = EPS_Katana;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<EWalkState> WalkState;


	UFUNCTION(BlueprintCallable)
		FVector GetDesiredMovementInput();

	UFUNCTION(BlueprintCallable)
		void RotateChara(float DeltaTime);



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
