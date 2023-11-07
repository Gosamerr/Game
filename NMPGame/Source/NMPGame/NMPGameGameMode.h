// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NMPGameState.h"
#include "NMPGameGameMode.generated.h"


UCLASS(minimalapi)
class ANMPGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANMPGameGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetDecayRate();

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWinMultiplier();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Power")
		float PowerDrainDelay;

	FTimerHandle PowerDrainTimer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float DecayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float PowerToWinMultiplier;


	int32 DeadPlayerCount;

private:
	void DrainPowerOverTime();

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	void HandleNewState(EBatteryPlayState NewState);
};



