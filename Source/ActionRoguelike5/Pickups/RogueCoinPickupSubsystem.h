// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RogueCoinPickupSubsystem.generated.h"

class ARoguePlayerCharacter;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE5_API URogueCoinPickupSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	void AddCoinPickups(const TArray<FVector>& NewLocations, const TArray<int32>& NewAmounts);
	
	void RemoveCoinPickup(int32 IndexToRemove);
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(RogueCoinPickupSubsystem, STATGROUP_Tickables); }
	
	void SpawnCoinsAtLocation(const FVector& LocationToSpawnAt, int32 CreditsAmount);
	
protected:
	
	void OnPickupMeshLoadComplete(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject);
	
	void OnPickupSoundLoadComplete(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject);
	
	void CheckCoinsPickupForPlayer(ARoguePlayerCharacter* PlayerCharacter);
	
	void PlayPickupSound();
	
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> MeshISM;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> WorldAudioComp;
	
	// Cached Param from DevSettings for Audio Comp Pickups
	FName CoinPickupTriggerParamName;
	
	TArray<FVector> CoinLocations;
	TArray<int32> CoinAmounts;
	TArray<FPrimitiveInstanceId> MeshIds;
};
