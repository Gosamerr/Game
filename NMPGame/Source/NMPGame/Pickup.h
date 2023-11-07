// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class NMPGAME_API APickup : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	APickup();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Returns whether or not pickup is active
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	//allows other classes safely change pickup`s activation state
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
	void WasCollected();
	virtual void WasCollected_Implementation();

	UFUNCTION(BlueprintAuthorityOnly, Category = "Pickup")
	virtual void PickedUpBy(APawn* Pawn);

protected:

	//True when the pickup can be used, false when pickup is deactivated
	UPROPERTY(ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;

	// This is called whenever bIsActive updated
	UFUNCTION()
	virtual void OnRep_IsActive();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	APawn* PickupInstigator;
private:

	UFUNCTION(NetMulticast, Unreliable)
		void ClientOnPickedUpBy(APawn* Pawn);
};
