// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class NMPGAME_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
public:
	ABatteryPickup();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void PickedUpBy(APawn* Pawn) override;

	float GetPower();

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = true))
		float BatteryPower;
};
