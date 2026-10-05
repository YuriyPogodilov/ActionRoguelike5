// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePrimaryGameMode.h"

#include "ActionRoguelike5.h"
#include "RogueGameTypes.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "AI/RogueAICharacter.h"
#include "AI/RogueMonsterData.h"
#include "Core/RogueGameInstance.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/GameplayStatics.h"


TAutoConsoleVariable<bool> CVarGameBotSpawningEnabled(
	TEXT("game.BotSpawningEnabled"),
	true,
	TEXT("Allow disabling of bot spawning for debugging purposes."),
	ECVF_Cheat);

TAutoConsoleVariable<int32> CVarGameBotLimit(
	TEXT("game.BotLimit"),
	5,
	TEXT("Define the maximum number of alive bots in the world."),
	ECVF_Default);


ARoguePrimaryGameMode::ARoguePrimaryGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ARoguePrimaryGameMode::StartPlay()
{
	Super::StartPlay();

	FRandomStream GlobalRandomStream(GlobalStartingSeed);
	
	for (FRogueDirectorData& Director : Directors)
	{
		int32 NewSeed = GlobalRandomStream.RandRange(0, MAX_int32 - 1);
		Director.RandomStream_MonsterSelection = FRandomStream(NewSeed);
		
		UE_LOG(LogGameMode, Log, TEXT("Seed: %d for %s"), Director.RandomStream_MonsterSelection.GetInitialSeed(), *Director.DebugDisplayName);
	}
}

void ARoguePrimaryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	float TotalElapsedTime = GetWorld()->TimeSeconds;

	int32 KeyID = ONSCREENDEBUGKEY_SPAWNDIRECTOR;
	for (FRogueDirectorData& Director : Directors)
	{
		if (!IsValid(Director.MonsterSpawnDataTable))
		{
			continue;
		}
		
		float CreditPerSecond = Director.CreditGainCurve.GetRichCurve()->Eval(TotalElapsedTime);
		Director.CurrentCredits += CreditPerSecond * DeltaSeconds;
		
		FString DebugMsg = FString::Printf(TEXT("%s\nCurrent Credits: %.2f\nNextTickTime: %.2f"), *Director.DebugDisplayName, Director.CurrentCredits, Director.NextTickTime);
		GEngine->AddOnScreenDebugMessage(KeyID, PrimaryActorTick.TickInterval, Director.DebugColor, DebugMsg);
		KeyID++;
		
		if (Director.NextTickTime > TotalElapsedTime)
		{
			continue;
		}
		
		bool bSuccess = TrySpawnMonster(Director);
		
		Director.NextTickTime = TotalElapsedTime + (bSuccess ? Director.TickInterval : Director.TimeBetweenWaves);
	}
}

bool ARoguePrimaryGameMode::TrySpawnMonster(FRogueDirectorData& Director)
{
	URogueGameInstance* GI = GetGameInstance<URogueGameInstance>();
	const int32 MaxBotLimit = CVarGameBotLimit.GetValueOnGameThread();
	if (GI->AliveMonsters.Num() >= MaxBotLimit)
	{
		UE_LOG(LogGameMode, Log, TEXT("Reached bot spawn limit of %d"), MaxBotLimit);
		return false;
	}
	
	TArray<FMonsterSpawnData*> AllRows;
	Director.MonsterSpawnDataTable->GetAllRows("SelectMonster", AllRows);
	
	float TotalWeight{ 0 };
	for (FMonsterSpawnData* Row : AllRows)
	{
		TotalWeight += Row->SpawnWeight;
	}
	
	float SelectedWeight = Director.RandomStream_MonsterSelection.FRandRange(0.0f, TotalWeight);

	FMonsterSpawnData* SelectedRow = nullptr;
	TotalWeight = 0.0f;
	for (FMonsterSpawnData* Row : AllRows)
	{
		TotalWeight += Row->SpawnWeight;
		if (SelectedWeight <= TotalWeight)
		{
			SelectedRow = Row;
			break;
		}
	}
	
	if (Director.CurrentCredits < SelectedRow->SpawnCost)
	{
		UE_LOG(LogGameMode, Log, TEXT("Not enough credits to spawn monster %s"), *SelectedRow->MonsterData.GetAssetName());
		return false;
	}
	
	Director.CurrentCredits -= SelectedRow->SpawnCost;
	
	FQueryFinishedSignature CompletedDelegate = FQueryFinishedSignature::CreateUObject(this, &ThisClass::OnSpawnQueryCompleted, SelectedRow);
	
	FEnvQueryRequest Request(Director.SpawnLocationQuery, this);
	int32 QueryIndex = Request.Execute(EEnvQueryRunMode::SingleResult, CompletedDelegate);
	
	return QueryIndex != INDEX_NONE;
}

void ARoguePrimaryGameMode::OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> QueryResult, FMonsterSpawnData* SpawnData)
{
	FVector SpawnLocation = QueryResult->GetItemAsLocation(0);
	
	SpawnData->MonsterData.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnMonsterClassLoaded, SpawnLocation, SpawnData));
}

void ARoguePrimaryGameMode::OnMonsterClassLoaded(const FSoftObjectPath& LoadedObjectPath, UObject* LoadedObject, FVector SpawnLocation, FMonsterSpawnData* SpawnData)
{
	if (!CVarGameBotSpawningEnabled.GetValueOnGameThread())
	{
		return;
	}
	
	FTransform SpawnTransform = FTransform(SpawnLocation);
	
	URogueMonsterData* MonsterData = SpawnData->MonsterData.Get();
	
	ARogueAICharacter* NewMonster = GetWorld()->SpawnActorDeferred<ARogueAICharacter>(MonsterData->MonsterClass, FTransform::Identity);
	
	NewMonster->SetMonsterData(MonsterData);
	
	UGameplayStatics::FinishSpawningActor(NewMonster, SpawnTransform);
	
	if (IsValid(NewMonster))
	{
		URogueActionSystemComponent* ActionComp = NewMonster->GetActionSystemComponent();
		for (const TSubclassOf<URogueAction>& ActionClass : MonsterData->Actions)
		{
			ActionComp->GrantAction(ActionClass);
		}
	}
	
	UE_VLOG_SPHERE(this, LogGameMode, Log, SpawnLocation, 32.0f, FColor::Blue, TEXT("Monster type:%s\nCost:%.2f"),
		*GetNameSafe(MonsterData->MonsterClass), SpawnData->SpawnCost);
}
