// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_PlayerCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// SetEnum


// Sets default values
AGS_PlayerCharacter::AGS_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 93.0f);

	TurnRateGamepad = 50.f;

	HealthHUD = nullptr;
	HealthHUDClass = nullptr;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 220.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;
	

}


// Called when the game starts or when spawned
void AGS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ActorsToIgnore.Add(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsLocallyControlled() && HealthHUDClass) 
	{
		APlayerController* FPC = GetController<APlayerController>();
		HealthHUD = CreateWidget<UUserWidget>(FPC, HealthHUDClass);
		HealthHUD->AddToPlayerScreen();
	}
}


// Called every frame
void AGS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//RotateChara(DeltaTime);
}

// Called to bind functionality to input
void AGS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	// PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	// PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGS_PlayerCharacter::Dodge);

	PlayerInputComponent->BindAction("ChangeButtle", IE_Pressed, this, &AGS_PlayerCharacter::ChangeButtle);

	//PlayerInputComponent->BindAction("Finish", IE_Pressed, this, &AGS_PlayerCharacter::DetectEnemy);

	PlayerInputComponent->BindAction("Attack_L", IE_Pressed, this, &AGS_PlayerCharacter::AttackLeft);
	PlayerInputComponent->BindAction("Attack_L", IE_Released, this, &AGS_PlayerCharacter::AttackEnd);
	//Aiming&Block
	PlayerInputComponent->BindAction("Aiming&Block", IE_Pressed, this, &AGS_PlayerCharacter::AimingorBlock);
	PlayerInputComponent->BindAction("Aiming&Block", IE_Released, this, &AGS_PlayerCharacter::EndofRight);

	PlayerInputComponent->BindAction("RainShoot", IE_Pressed, this, &AGS_PlayerCharacter::RainShoots);
	PlayerInputComponent->BindAction("RainShoot", IE_Released, this, &AGS_PlayerCharacter::RainShootEnd);
	PlayerInputComponent->BindAction("BurstShoot", IE_Pressed, this, &AGS_PlayerCharacter::BurstShoots);
	PlayerInputComponent->BindAction("BurstShoot", IE_Released, this, &AGS_PlayerCharacter::BurstShootEnd);
	PlayerInputComponent->BindAction("FastShoot", IE_Pressed, this, &AGS_PlayerCharacter::FastShoots);
	PlayerInputComponent->BindAction("FastShoot", IE_Released, this, &AGS_PlayerCharacter::FastShootEnd);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGS_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGS_PlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AGS_PlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AGS_PlayerCharacter::LookUpAtRate);
}

void AGS_PlayerCharacter::MoveForward(float Value)
{
	MoveForwardValue = Value;
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
	}
	
}

void AGS_PlayerCharacter::MoveRight(float Value)
{
	MoveRightValue = Value;
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
	}
}

void AGS_PlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AGS_PlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AGS_PlayerCharacter::ChangeButtle()
{
	if (!GetCharacterMovement()->IsFalling()) 
	{
		ChangeToApose = !ChangeToApose;
	}
}

void AGS_PlayerCharacter::AttackLeft()
{
	if (ChangeToApose && ChangeComplete) 
	{
		if (Weapon == EPS_Katana) {
			if(ChangeAttackCombo)
			{
				if (IsAttack)
				{
					SaveAttack = true;
				}
				else
				{
					IsAttack = true;
					switch (AttackCount)
					{
					case 0:
						AttackName = "AttackL1";
						AttackCount = 1;
						AttackDamage = 10;
						PlayAnimMontage(Attack10, 1.0f, NAME_None);
						break;
					case 1:
						AttackName = "AttackL2";
						AttackCount = 2;
						AttackDamage = 11;
						PlayAnimMontage(Attack11, 1.0f, NAME_None);
						break;
					case 2:
						AttackName = "AttackL3";
						AttackCount = 3;
						AttackDamage = 8;
						PlayAnimMontage(Attack12, 1.0f, NAME_None);
						break;
					case 3:
						AttackName = "AttackL4";
						AttackCount = 4;
						AttackDamage = 15;
						PlayAnimMontage(Attack13, 1.0f, NAME_None);
						break;
					case 4:
						AttackName = "AttackL5";
						AttackCount = 5;
						AttackDamage = 15;
						PlayAnimMontage(Attack14, 1.0f, NAME_None);
						break;
					case 5:
						AttackName = "AttackL6";
						AttackCount = 6;
						AttackDamage = 15;
						PlayAnimMontage(Attack15, 1.0f, NAME_None);
						break;
					default:
						break;
					}
				}
			}
			else
			{
				if (IsAttack)
				{
					SaveAttack = true;
				}
				else
				{
					IsAttack = true;
					switch (AttackCount)
					{
					case 0:
						AttackName = "AttackL1";
						AttackCount = 1;
						AttackDamage = 10;
						PlayAnimMontage(Attack00, 1.0f, NAME_None);
						break;
					case 1:
						AttackName = "AttackL2";
						AttackCount = 2;
						AttackDamage = 11;
						PlayAnimMontage(Attack01, 1.0f, NAME_None);
						break;
					case 2:
						AttackName = "AttackL3";
						AttackCount = 3;
						AttackDamage = 8;
						PlayAnimMontage(Attack02, 1.0f, NAME_None);
						break;
					case 3:
						AttackName = "AttackL4";
						AttackCount = 4;
						AttackDamage = 15;
						PlayAnimMontage(Attack03, 1.0f, NAME_None);
					default:
						break;
					}
				}
			}
		}
		else 
		{
			if (AbleToAim) 
			{
				ShootTap = true;
			}
		}
		
	}
}

void AGS_PlayerCharacter::AttackEnd()
{
	ShootTap = false;
}

void AGS_PlayerCharacter::AimingorBlock()
{
	if(!GetCharacterMovement()->IsFalling())
	{
		if(Weapon == EPS_Katana)
		{
			AbleToBlock = true;
		}else
		{
			AbleToAim = true;
			CameraBoom->CameraLagSpeed = 20.f;
		}
	}
}

void AGS_PlayerCharacter::CameraMovement(float Length,FVector Location)
{
	CameraBoom->TargetArmLength = Length;
	CameraBoom->SetRelativeLocation(Location);
}

void AGS_PlayerCharacter::EndofRight()
{
	AbleToAim = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	CameraBoom->CameraLagSpeed = 10.f;
	AbleToBlock = false;
}

void AGS_PlayerCharacter::RainShoots()
{
	RainShoot = true;
}

void AGS_PlayerCharacter::RainShootEnd()
{
	RainShoot = false;
}

void AGS_PlayerCharacter::BurstShoots()
{
	BurstShoot = true;
}

void AGS_PlayerCharacter::BurstShootEnd()
{
	BurstShoot = false;
}

void AGS_PlayerCharacter::FastShoots()
{
	if (AbleToAim && !FastShoot && PowerOn) {
		FastShoot = true;
	}
	else 
	{
		FastShoot = false;
	}
}

void AGS_PlayerCharacter::FastShootEnd()
{
	FastShoot = false;
}

void AGS_PlayerCharacter::Dodge()
{
	if (!IsDodge && ChangeToApose) {
		IsDodge = true;
	}
}

void AGS_PlayerCharacter::UpdateDirection(float ForwardValue,float RightValue)
{
	FRotator RotationValue0;
	FRotator RotationValue1;
	RotationValue0 = UKismetMathLibrary::NormalizedDeltaRotator(FollowCamera->GetComponentRotation(),GetCapsuleComponent()->GetComponentRotation());
	RotationValue1 = UKismetMathLibrary::NormalizedDeltaRotator(RotationValue0,UKismetMathLibrary::MakeRotFromX(FVector(ForwardValue, (RightValue*-1), 0)));
	DirectionValue = RotationValue1.Yaw;
}

void AGS_PlayerCharacter::ComboAttackSave()
{
	if(ChangeAttackCombo)
	{
		if (SaveAttack)
		{
			SaveAttack = false;
			switch (AttackCount)
			{
			case 0:
				AttackName = "AttackL1";
				AttackCount = 1;
				AttackDamage = 10;
				PlayAnimMontage(Attack10, 1.0f, NAME_None);
				break;
			case 1:
				AttackName = "AttackL2";
				AttackCount = 2;
				AttackDamage = 11;
				PlayAnimMontage(Attack11, 1.0f, NAME_None);
				break;
			case 2:
				AttackName = "AttackL3";
				AttackCount = 3;
				AttackDamage = 8;
				PlayAnimMontage(Attack12, 1.0f, NAME_None);
				break;
			case 3:
				AttackName = "AttackL4";
				AttackCount = 4;
				AttackDamage = 15;
				PlayAnimMontage(Attack13, 1.0f, NAME_None);
				break;
			case 4:
				AttackName = "AttackL5";
				AttackCount = 5;
				AttackDamage = 15;
				PlayAnimMontage(Attack14, 1.0f, NAME_None);
				break;
			case 5:
				AttackName = "AttackL6";
				AttackCount = 6;
				AttackDamage = 15;
				PlayAnimMontage(Attack15, 1.0f, NAME_None);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		if (SaveAttack)
		{
			SaveAttack = false;
			switch (AttackCount)
			{
			case 0:
				AttackName = "AttackL1";
				AttackCount = 1;
				AttackDamage = 10;
				PlayAnimMontage(Attack00, 1.0f, NAME_None);
				break;
			case 1:
				AttackName = "AttackL2";
				AttackCount = 2;
				AttackDamage = 11;
				PlayAnimMontage(Attack01, 1.0f, NAME_None);
				break;
			case 2:
				AttackName = "AttackL3";
				AttackCount = 3;
				AttackDamage = 8;
				PlayAnimMontage(Attack02, 1.0f, NAME_None);
				break;
			case 3:
				AttackName = "AttackL4";
				AttackCount = 4;
				AttackDamage = 15;
				PlayAnimMontage(Attack03, 1.0f, NAME_None);
			default:
				break;
			}
		}
	}
	
}

void AGS_PlayerCharacter::ResetCombo()
{
	IsAttack = false;
	SaveAttack = false;
	AttackCount = 0;
}

TArray< FHitResult > AGS_PlayerCharacter::GetNearestEnemy()
{
	TArray< FHitResult > Hit;
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	const FVector Start = Player->GetActorLocation();
	const FVector End = Player->GetActorLocation() + FVector(0.1,0.1,0.1);
	bool isHit = UKismetSystemLibrary::SphereTraceMulti(
	GetWorld(),
	Start,
	End,
	1000.f,
	UEngineTypes::ConvertToTraceType((ECC_Pawn)),
	false,
	ActorsToIgnore,
	EDrawDebugTrace::None,
	Hit,
	true,
	FLinearColor::Red,
	FLinearColor::Green,
	5.0f
	);
	return Hit;
}

void AGS_PlayerCharacter::ChangeIndex(bool BeginDis, int index, int arrayLength)
{
	if(index<arrayLength && BeginDis)
	{
		++index;
		if(index == arrayLength)
		{
			index -= 2;
			BeginDis = false;
		}
	}
	else
	{
		--index;
		if(index == 0)
		{
			BeginDis = true;
		}
	}
}


void AGS_PlayerCharacter::TraceEvent(USceneComponent* Actor,bool &IsHit,FHitResult &Hit)
{
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Actor->GetSocketLocation("Bottom"),
		Actor->GetSocketLocation("Top"),
		5.0f,
		UEngineTypes::ConvertToTraceType((ECC_Pawn)),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
		);
}

AActor* AGS_PlayerCharacter::DetectEnemy()
{
	bool IsHit = false;
	FHitResult Hit;
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	FVector start = Player->GetActorLocation();
	FVector end = Player->GetActorForwardVector() * 100.f+Player->GetActorLocation();
	IsHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
		);
		return Hit.GetActor();
}

float AGS_PlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageCaused = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageCaused = DamageCaused*UKismetMathLibrary::RandomFloatInRange(1.01f,1.05f)*(UKismetMathLibrary::Log(PS_Nax,10))/(1+PS_Def);
	DamageCaused = FMath::Min(Health, DamageCaused);
	if(AbleToBlock)
	{
		Health -= (DamageCaused*0.1f);
		DamageCounts += DamageCaused;
		if(DamageCounts >= 50)
		{
			IsBroken = true;
			DamageCounts = 0;
		}
	}
	else
	{
		Health -= DamageCaused;
	}
	if (Health <= 0)
	{
		DisableInput(GetWorld()->GetFirstPlayerController());
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageCaused;
}


FVector AGS_PlayerCharacter::GetDesiredMovementInput()
{
	FVector VelocityForward;
	FVector VelocityRight;

	VelocityForward = UKismetMathLibrary::GetForwardVector(Controller->GetControlRotation());
	VelocityForward = MoveForwardValue * VelocityForward;
	VelocityRight = UKismetMathLibrary::GetRightVector(Controller->GetControlRotation());
	VelocityRight = MoveRightValue * VelocityRight;

	return UKismetMathLibrary::Normal(VelocityForward + VelocityRight, 0.0001f);
}		/*Don't use this*/

void AGS_PlayerCharacter::RotateChara(float DeltaTime)
{
	FVector Velocity;
	Velocity = GetVelocity();

	if (Velocity.Size() > 100) 
	{
		FRotator MovementInput;
		MovementInput = UKismetMathLibrary::MakeRotFromX(GetDesiredMovementInput());
		MovementInput = UKismetMathLibrary::RInterpTo(GetActorRotation(), MovementInput, DeltaTime,1.0f);
		SetActorRotation(FRotator(0,0, MovementInput.Yaw));
	}
}   /*Don't use this*/
