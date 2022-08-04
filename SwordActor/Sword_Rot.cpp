// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword_Rot.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"



// Sets default values
ASword_Rot::ASword_Rot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	RotationPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RotationPoint"));
	RotationPoint->SetupAttachment(Root);

	Sword0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword0"));
	Sword0->SetupAttachment(RotationPoint);
	
	Sword1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword1"));
	Sword1->SetupAttachment(RotationPoint);
	
	Sword2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword2"));
	Sword2->SetupAttachment(RotationPoint);
	
	Sword3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword3"));
	Sword3->SetupAttachment(RotationPoint);
	
	Sword4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword4"));
	Sword4->SetupAttachment(RotationPoint);

}

// Called when the game starts or when spawned
void ASword_Rot::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASword_Rot::RotationSelf, .01f, true);

}

// Called every frame
void ASword_Rot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword_Rot::RotationSelf()
{
	FRotator NewRotation = FRotator(0, 1, 0);
	AddActorLocalRotation(NewRotation, false, 0, ETeleportType::None);
}

