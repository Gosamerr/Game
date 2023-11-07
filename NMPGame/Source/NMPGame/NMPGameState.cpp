// Fill out your copyright notice in the Description page of Project Settings.


#include "NMPGameState.h"
#include "NMPGame.h"
#include "Net/UnrealNetwork.h"

ANMPGameState::ANMPGameState()
{
	CurrentState = EBatteryPlayState::EUnknown;
}

void ANMPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANMPGameState, PowerToWin);
	DOREPLIFETIME(ANMPGameState, CurrentState);
	DOREPLIFETIME(ANMPGameState, WinningPlayerName);

}

EBatteryPlayState ANMPGameState::GetCurrentState() const
{
	return CurrentState;
}

void ANMPGameState::SetCurrentState(EBatteryPlayState NewState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentState = NewState;
	}
}

void ANMPGameState::OnRep_CurrentState()
{

}