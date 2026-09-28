// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinTestActor.h"

#include "NavigationSystem.h"
#include "Pickups/RogueCoinPickupSubsystem.h"


ARogueCoinTestActor::ARogueCoinTestActor()
{
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
}

void ARogueCoinTestActor::SpawnCoins(int32 SpawnCount)
{
	TArray<FVector> CoinLocations;
	TArray<int32> CoinAmounts;
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
	FVector ActorLocation = GetActorLocation();

	for (int i = 0; i < SpawnCount; ++i)
	{
		FNavLocation NavLocation;
		NavSystem->GetRandomPointInNavigableRadius(ActorLocation, 1024.0f, NavLocation);
		
		CoinLocations.Add(NavLocation.Location);
		CoinAmounts.Add(10);
	}
	
	URogueCoinPickupSubsystem* CoinSystem = GetWorld()->GetSubsystem<URogueCoinPickupSubsystem>();
	CoinSystem->AddCoinPickups(CoinLocations, CoinAmounts);
}
