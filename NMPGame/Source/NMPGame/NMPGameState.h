// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NMPGameState.generated.h"

UENUM(BlueprintType)
enum EBatteryPlayState
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};
/**
 * 
 */
UCLASS()
class NMPGAME_API ANMPGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	ANMPGameState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Power")
	float PowerToWin;

	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState() const;

	void SetCurrentState(EBatteryPlayState NewState);

	UFUNCTION()
	void OnRep_CurrentState();

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Power")
	FString WinningPlayerName;

private:

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
	TEnumAsByte<enum EBatteryPlayState> CurrentState;
};
