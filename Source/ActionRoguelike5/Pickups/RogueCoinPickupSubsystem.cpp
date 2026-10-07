// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinPickupSubsystem.h"

#include "ActionRoguelike5.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/RogueDeveloperSettings.h"
#include "Player/RoguePlayerCharacter.h"
#include "ProfilingDebugging/CountersTrace.h"

TRACE_DECLARE_INT_COUNTER(CoinInstanceCount, TEXT("Coins in world"));

void URogueCoinPickupSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	MeshISM = Cast<UInstancedStaticMeshComponent>(NewObject<UInstancedStaticMeshComponent>(&InWorld, NAME_None, RF_Transient));
	MeshISM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshISM->SetAffectDistanceFieldLighting(false);
	MeshISM->RegisterComponentWithWorld(&InWorld);
	
	const URogueDeveloperSettings* DevSettings = GetDefault<URogueDeveloperSettings>();
	CoinPickupTriggerParamName = DevSettings->CoinPickupTriggerParameter;
	
	DevSettings->CoinPickupMesh.LoadAsync(
		FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnPickupMeshLoadComplete));
	
	WorldAudioComp = Cast<UAudioComponent>(NewObject<UAudioComponent>(&InWorld, NAME_None, RF_Transient));
	WorldAudioComp->SetAutoActivate(false);
	WorldAudioComp->bAllowSpatialization = false;
	WorldAudioComp->RegisterComponentWithWorld(&InWorld);
	
	DevSettings->CoinPickupSound.LoadAsync(
		FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnPickupSoundLoadComplete));
	
	TRACE_COUNTER_SET(CoinInstanceCount, 0);
}

void URogueCoinPickupSubsystem::OnPickupMeshLoadComplete(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject)
{
	MeshISM->SetStaticMesh(Cast<UStaticMesh>(LoadedObject));
}

void URogueCoinPickupSubsystem::OnPickupSoundLoadComplete(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject)
{
	WorldAudioComp->SetSound(Cast<USoundBase>(LoadedObject));
}

void URogueCoinPickupSubsystem::PlayPickupSound()
{
	if (!WorldAudioComp->IsPlaying())
	{
		WorldAudioComp->Play();
	}
	
	WorldAudioComp->SetTriggerParameter(CoinPickupTriggerParamName);
}

void URogueCoinPickupSubsystem::AddCoinPickups(const TArray<FVector>& NewLocations, const TArray<int32>& NewAmounts)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::AddCoinPickups)
	
	CoinLocations.Append(NewLocations);
	CoinAmounts.Append(NewAmounts);
	
	TArray<FTransform> MeshTransforms;
	for (int i = 0; i < NewLocations.Num(); ++i)
	{
		MeshTransforms.Add(FTransform(NewLocations[i] + FVector(0.0f, 0.0f, 50.0f)));
	}
	
	TArray<FPrimitiveInstanceId> NewMeshIds = MeshISM->AddInstancesById(MeshTransforms, true, false);
	MeshIds.Append(NewMeshIds);
	
	TRACE_COUNTER_SET(CoinInstanceCount, CoinLocations.Num());
}

void URogueCoinPickupSubsystem::RemoveCoinPickup(int32 IndexToRemove)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::RemoveCoinPickup)
	
	CoinLocations.RemoveAtSwap(IndexToRemove, EAllowShrinking::No);
	CoinAmounts.RemoveAtSwap(IndexToRemove, EAllowShrinking::No);
	
	MeshISM->RemoveInstanceById(MeshIds[IndexToRemove]);
	MeshIds.RemoveAtSwap(IndexToRemove, EAllowShrinking::No);
	
	TRACE_COUNTER_SET(CoinInstanceCount, CoinLocations.Num());
}

void URogueCoinPickupSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick)
	
	for (auto PlayerCharacter : TActorRange<ARoguePlayerCharacter>(GetWorld()))
	{
		CheckCoinsPickupForPlayer(PlayerCharacter);
	}
}

void URogueCoinPickupSubsystem::CheckCoinsPickupForPlayer(ARoguePlayerCharacter* PlayerCharacter)
{
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	
	TArray<int32> ProcessList;
	
	const float PickupRadius = 200.f;
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick::DistanceCheck)
		
		for (int i = 0; i < CoinLocations.Num(); ++i)
		{
			float Distance = FVector::Dist(PlayerLocation, CoinLocations[i]);
			if (Distance < PickupRadius)
			{
				ProcessList.Add(i);
			}
		}
	}

	int32 TotalCoinsToGrant = 0;
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick::PickupHandle)
		
		for (int i = ProcessList.Num() - 1; i >= 0; --i)
		{
			int32 CoinIndex = ProcessList[i];
		
			TotalCoinsToGrant += CoinAmounts[CoinIndex];
		
			RemoveCoinPickup(CoinIndex);
		}
	}
	
	if (TotalCoinsToGrant > 0)
	{
		PlayPickupSound();
		
		URogueActionSystemComponent* ActionComp = PlayerCharacter->GetComponentByClass<URogueActionSystemComponent>();
		check(ActionComp);
	
		ActionComp->ApplyAttributeChange(SharedGameplayTags::Attribute_Credits, TotalCoinsToGrant, EAttributeModifyType::Base);
		
		UE_LOG(LogGame, Log, TEXT("Picked up Coins amount = %d"), TotalCoinsToGrant);
	}
}

void URogueCoinPickupSubsystem::SpawnCoinsAtLocation(const FVector& LocationToSpawnAt, int32 CreditsAmount)
{
	TArray<FVector> NewCoinLocations;
	TArray<int32> NewCoinAmounts;

	int32 CreditsToGive = 0;
	while (CreditsAmount > CreditsToGive)
	{
		int32 MinCreditsAmountInCoin = 1;
		int32 MaxCreditsAmountInCoin = 10;
		
		int32 NewAmount = FMath::RandRange(MinCreditsAmountInCoin, MaxCreditsAmountInCoin);
		
		if (CreditsToGive + NewAmount > CreditsAmount)
		{
			NewAmount -= CreditsToGive + NewAmount - CreditsAmount;
		}
		
		CreditsToGive += NewAmount;
		NewCoinAmounts.Add(NewAmount);
	}

	int32 ControlSum = 0;
	for (int i = 0; i < NewCoinAmounts.Num(); ++i)
	{
		ControlSum += NewCoinAmounts[i];
	}
	ensure(ControlSum == CreditsAmount);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
	
	for (int i = 0; i < NewCoinAmounts.Num(); ++i)
	{
		FNavLocation NavLocation;
		NavSystem->GetRandomPointInNavigableRadius(LocationToSpawnAt, 512.0f, NavLocation);
		
		NewCoinLocations.Add(NavLocation.Location);
	}
	
	AddCoinPickups(NewCoinLocations, NewCoinAmounts);
}
