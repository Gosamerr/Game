 // Fill out your copyright notice in the Description page of Project Settings.
#include "Pickup.h"
#include "NMPGame.h"
#include "Net/Unrealnetwork.h"


APickup::APickup()
{
	//Tell UE4 to replicate this actor
	bReplicates = true;

	//Pickups do not need to tick every frame
	PrimaryActorTick.bCanEverTick = false;

	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

	if (GetLocalRole() == ROLE_Authority)
	{
		bIsActive = true;
	}
}

void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APickup, bIsActive);
}

bool APickup::IsActive()
{
	return bIsActive;
}

void APickup::SetActive(bool NewPickupState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsActive = NewPickupState;
	}
}

void APickup::WasCollected_Implementation()
{
	UE_LOG(LogClass, Log, TEXT("APickup::WasCollected_Implementation %s"), *GetName());
}

void APickup::PickedUpBy(APawn* Pawn)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		PickupInstigator = Pawn;

		ClientOnPickedUpBy(Pawn);
	}
}

void APickup::ClientOnPickedUpBy_Implementation(APawn* Pawn)
{
	PickupInstigator = Pawn;

	WasCollected();

}

void APickup::OnRep_IsActive()
{
	
}
