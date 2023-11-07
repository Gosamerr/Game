// Copyright Epic Games, Inc. All Rights Reserved.

#include "NMPGame.h"
#include "NMPGameGameMode.h"
#include "NMPGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "NMPGameState.h"
#include "SpawnVolume.h"

ANMPGameGameMode::ANMPGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClass(TEXT("/Game/Blueprints/BP_NMPGameHUD"));
	if (PlayerHUDClass.Class != NULL)
	{
		HUDClass = PlayerHUDClass.Class;
	}

	GameStateClass = ANMPGameState::StaticClass();

	DecayRate = 0.02f;

	PowerDrainDelay = 0.25f;

	PowerToWinMultiplier = 1.25f;

	DeadPlayerCount = 0;
}

void ANMPGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	check(World);
	ANMPGameState* MyGameState = Cast<ANMPGameState>(GameState);
	check(MyGameState);

	DeadPlayerCount = 0;

	TArray<AActor*>FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ASpawnVolume::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		if (ASpawnVolume* TestSpawnVol = Cast<ASpawnVolume>(Actor))
		{
			SpawnVolumeActors.AddUnique(TestSpawnVol);
		}
	}

	HandleNewState(EBatteryPlayState::EPlaying);

	for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(*It))
		{
			if (ANMPGameCharacter* BatteryCharacter = Cast<ANMPGameCharacter>(PlayerController->GetPawn()))
			{
				MyGameState->PowerToWin = BatteryCharacter->GetInitialPower() * PowerToWinMultiplier;
				break;
			}
		}
	}
}

float ANMPGameGameMode::GetDecayRate()
{
	return DecayRate;
}

float ANMPGameGameMode::GetPowerToWinMultiplier()
{
	return PowerToWinMultiplier;
}

void ANMPGameGameMode::DrainPowerOverTime()
{
	UWorld* World = GetWorld();
	check(World);
	ANMPGameState* MyGameState = Cast<ANMPGameState>(GameState);
	check(MyGameState);

	for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It)
	{
		if(APlayerController* PlayerController = Cast<APlayerController>(*It))
		{
			if (ANMPGameCharacter* BatteryCharacter = Cast<ANMPGameCharacter>(PlayerController->GetPawn()))
			{
				if (BatteryCharacter->GetCurrentPower() > MyGameState->PowerToWin)
				{
					MyGameState->WinningPlayerName = BatteryCharacter->GetName();
					HandleNewState(EBatteryPlayState::EWon);
				}
				else if (BatteryCharacter->GetCurrentPower() > 0)
				{
					BatteryCharacter->UpdatePower(-PowerDrainDelay*DecayRate*(BatteryCharacter->GetInitialPower()));
				}
				else
				{
					BatteryCharacter->OnPlayerDeath();
					++DeadPlayerCount;

					if (DeadPlayerCount >= GetNumPlayers())
					{
						HandleNewState(EBatteryPlayState::EGameOver);
					}
				}
				
			}
		
		}
	}

}

void ANMPGameGameMode::HandleNewState(EBatteryPlayState NewState)
{
	UWorld* World = GetWorld();
	check(World);
	ANMPGameState* MyGameState = Cast<ANMPGameState>(GameState);
	check(MyGameState);
	
	if (NewState != MyGameState->GetCurrentState())
	{
		MyGameState->SetCurrentState(NewState);

		switch (NewState)
		{
		case EBatteryPlayState::EPlaying:
			for (ASpawnVolume* SpawnVol : SpawnVolumeActors)
			{
				SpawnVol->SetSpawningActive(true);
			}
			GetWorldTimerManager().SetTimer(PowerDrainTimer, this, &ANMPGameGameMode::DrainPowerOverTime, PowerDrainDelay, true);
			break;
			
		case EBatteryPlayState::EGameOver:
			for (ASpawnVolume* SpawnVol : SpawnVolumeActors)
			{
				SpawnVol->SetSpawningActive(false);
			}
			GetWorldTimerManager().ClearTimer(PowerDrainTimer);
			break;

		case EBatteryPlayState::EWon:
			for (ASpawnVolume* SpawnVol : SpawnVolumeActors)
			{
				SpawnVol->SetSpawningActive(false);
			}
			GetWorldTimerManager().ClearTimer(PowerDrainTimer);
			break;

		default:
		case EBatteryPlayState::EUnknown:
			break;
		}
	}
}
