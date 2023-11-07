// Fill out your copyright notice in the Description page of Project Settings.

#include "NMPGame.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Pickup.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (GetLocalRole() == ROLE_Authority)
	{
		WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
		RootComponent = WhereToSpawn;


		SpawnDelayRangeLow = 1.0f;
		SpawnDelayRangeHigh = 4.5f;

	}
	

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	if (WhereToSpawn != NULL)
	{
		FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
		FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;
		return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
	}
	return FVector();
}

void ASpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bShouldSpawn)
		{
			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}
		else
		{ 
			GetWorldTimerManager().ClearTimer(SpawnTimer);
		}
	}
}

void ASpawnVolume::SpawnPickup()
{
	//if we are Server and we have something to spawn
	if (GetLocalRole() == ROLE_Authority && WhatToSpawn != NULL)
	{
		//check for valid world
		if (UWorld* const World = GetWorld())
		{
			//setup any required params/info
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			//where put pikup
			FVector SpawnLocation = GetRandomPointInVolume();

			//Set random rotation for the spawned pickup
			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 300.0f;
			SpawnRotation.Pitch = FMath::FRand() * 300.0f;
			SpawnRotation.Roll = FMath::FRand() * 300.0f;

			//drop new pickup in the world
			APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			SetSpawningActive(true);
		}
	}
}