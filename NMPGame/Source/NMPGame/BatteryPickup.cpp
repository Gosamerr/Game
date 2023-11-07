// Fill out your copyright notice in the Description page of Project Settings.

#include "NMPGame.h"
#include "BatteryPickup.h"
#include "Net/UnrealNetwork.h"

ABatteryPickup::ABatteryPickup()
{
	//Keep movement synced fron server to client
	SetReplicates(true);
	SetReplicateMovement(true);

	//This pickup is physics enabled and should move
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetSimulatePhysics(true);

	BatteryPower = 200.0f;
}

void ABatteryPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABatteryPickup, BatteryPower);
}

void ABatteryPickup::PickedUpBy(APawn* Pawn)
{
	Super::PickedUpBy(Pawn);

	if (GetLocalRole() == ROLE_Authority)
	{
		SetLifeSpan(2.0f);

	}
}

float ABatteryPickup::GetPower()
{
	return BatteryPower;
}
