// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinPickupSubsystem.h"

#include "ActionRoguelike5.h"
#include "EngineUtils.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Player/RoguePlayerCharacter.h"


void URogueCoinPickupSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	FSoftObjectPath MeshAssetPath(TEXT("/Game/ExampleContent/Meshes/SM_Pickup_Coin.SM_Pickup_Coin"));
	UStaticMesh* LoadedMesh = Cast<UStaticMesh>(MeshAssetPath.TryLoad()); 
	
	MeshISM = Cast<UInstancedStaticMeshComponent>(NewObject<UInstancedStaticMeshComponent>(&InWorld, NAME_None, RF_Transient));
	MeshISM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshISM->SetStaticMesh(LoadedMesh);
	MeshISM->RegisterComponentWithWorld(&InWorld);
}

void URogueCoinPickupSubsystem::AddCoinPickups(TArray<FVector> NewLocations, TArray<int32> NewAmounts)
{
	CoinLocations.Append(NewLocations);
	CoinAmounts.Append(NewAmounts);
	
	TArray<FTransform> MeshTransforms;
	for (int i = 0; i < NewLocations.Num(); ++i)
	{
		MeshTransforms.Add(FTransform(NewLocations[i] + FVector(0.0f, 0.0f, 50.0f)));
	}
	
	TArray<FPrimitiveInstanceId> NewMeshIds = MeshISM->AddInstancesById(MeshTransforms, true, false);
	MeshIds.Append(NewMeshIds);
}

void URogueCoinPickupSubsystem::RemoveCoinPickup(int32 IndexToRemove)
{
	CoinLocations.RemoveAt(IndexToRemove);
	CoinAmounts.RemoveAt(IndexToRemove);
	
	MeshISM->RemoveInstanceById(MeshIds[IndexToRemove]);
	MeshIds.RemoveAt(IndexToRemove);
}

void URogueCoinPickupSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UWorld* World = GetWorld();
	
	FVector PlayerLocation = FVector::ZeroVector;
	for (auto PlayerCharacter : TActorRange<ARoguePlayerCharacter>(World))
	{
		PlayerLocation = PlayerCharacter->GetActorLocation();
	}

	TArray<int32> ProcessList;
	
	const float PickupRadius = 200.f;
	
	for (int i = 0; i < CoinLocations.Num(); ++i)
	{
		float Distance = FVector::Dist(PlayerLocation, CoinLocations[i]);
		if (Distance < PickupRadius)
		{
			ProcessList.Add(i);
		}
	}

	int32 TotalCoinsToGrant = 0;
	
	for (int i = ProcessList.Num() - 1; i >= 0; --i)
	{
		int32 CoinIndex = ProcessList[i];
		
		TotalCoinsToGrant += CoinAmounts[CoinIndex];
		
		RemoveCoinPickup(CoinIndex);
	}
	
	// @todo: grant coins to player
	UE_CLOG(TotalCoinsToGrant > 0, LogGame, Log, TEXT("Picked up Coins amount = %d"), TotalCoinsToGrant);

	for (int i = 0; i < CoinLocations.Num(); ++i)
	{
		DrawDebugPoint(World, CoinLocations[i], 8.0f, FColor::White);
	}
}
