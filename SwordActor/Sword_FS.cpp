// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword_FS.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sword_Interface.h"

// Sets default values
ASword_FS::ASword_FS()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CreationPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CreationPoint"));
	CreationPoint->SetupAttachment(Root);

	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(CreationPoint);
	Sword->SetCollisionProfileName("Blade");

	Sword->OnComponentBeginOverlap.AddDynamic(this, &ASword_FS::BeginOverlap);

}

// Called when the game starts or when spawned
void ASword_FS::BeginPlay()
{
	Super::BeginPlay();

	PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	PlayerLocation.Z -= 100.f;

	GetWorldTimerManager().SetTimer(LocationTimerHandle, this, &ASword_FS::UpdateLocation, .02f, true, 2.f);
	GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ASword_FS::UpdateRotation, .02f, true, 2.f);

}

// Called every frame
void ASword_FS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword_FS::UpdateLocation()
{
	FVector Current;
	Current = Sword->GetComponentLocation();
	SetActorLocation(FMath::VInterpTo(Current, PlayerLocation, GetWorld()->GetDeltaSeconds(),10.0f), false);
}

void ASword_FS::UpdateRotation()
{
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Sword->GetComponentLocation(), PlayerLocation));
}

void ASword_FS::ClearTimer()
{
	GetWorldTimerManager().ClearTimer(LocationTimerHandle);
	//Destroy();
}

void ASword_FS::BeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherActor != nullptr))
	{

		if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) 
		{
			GetWorldTimerManager().ClearTimer(RotationTimerHandle);
			ISword_Interface::Execute_UpdateControlPermissions(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), this);
			GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &ASword_FS::ClearTimer, 2.f, false);
		}
		else
		{
			GetWorldTimerManager().ClearTimer(RotationTimerHandle);
			GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &ASword_FS::ClearTimer, 2.f, false);
		}
		
	}
}
