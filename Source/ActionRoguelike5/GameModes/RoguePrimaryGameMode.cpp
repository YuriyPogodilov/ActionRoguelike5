// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePrimaryGameMode.h"

#include "RogueGameTypes.h"
#include "AI/RogueAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

ARoguePrimaryGameMode::ARoguePrimaryGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ARoguePrimaryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	float TotalElapsedTime = GetWorld()->TimeSeconds;

	for (FRogueDirectorData& Director : Directors)
	{
		if (!IsValid(Director.MonsterSpawnDataTable))
		{
			return;
		}
		
		float CreditPerSecond = Director.CreditGainCurve.GetRichCurve()->Eval(TotalElapsedTime);
		Director.CurrentCredits += CreditPerSecond * DeltaSeconds;
		
		if (Director.NextTickTime > TotalElapsedTime)
		{
			continue;
		}
		
		bool bSuccess = TrySpawnMonster(Director);
		
		Director.NextTickTime = TotalElapsedTime + (bSuccess ? Director.TickInterval : Director.TimeBetweenWaves);
		
		UE_LOG(LogGameMode, Log, TEXT("Total Credits: %f"), Director.CurrentCredits);
	}
}

bool ARoguePrimaryGameMode::TrySpawnMonster(FRogueDirectorData& Director)
{
	TArray<FMonsterSpawnData*> AllRows;
	Director.MonsterSpawnDataTable->GetAllRows("SelectMonster", AllRows);
	
	int32 SelectedIndex = FMath::RandRange(0, AllRows.Num() - 1);
	FMonsterSpawnData* SelectedData = AllRows[SelectedIndex];
	
	if (Director.CurrentCredits < SelectedData->SpawnCost)
	{
		UE_LOG(LogGameMode, Log, TEXT("Not enough credits to spawn monster %s"), *SelectedData->MonsterClass.GetAssetName());
		return false;
	}
	
	FQueryFinishedSignature CompletedDelegate = FQueryFinishedSignature::CreateUObject(this, &ThisClass::OnSpawnQueryCompleted, SelectedData);
	
	FEnvQueryRequest Request(Director.SpawnLocationQuery, this);
	int32 QueryIndex = Request.Execute(EEnvQueryRunMode::SingleResult, CompletedDelegate);
	
	return QueryIndex != INDEX_NONE;
}

void ARoguePrimaryGameMode::OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> QueryResult, FMonsterSpawnData* SpawnData)
{
	FVector SpawnLocation = QueryResult->GetItemAsLocation(0);
	
	SpawnData->MonsterClass.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnMonsterClassLoaded, SpawnLocation, SpawnData));
}

void ARoguePrimaryGameMode::OnMonsterClassLoaded(const FSoftObjectPath& LoadedObjectPath, UObject* LoadedObject, FVector SpawnLocation, FMonsterSpawnData* SpawnData)
{
	FActorSpawnParameters SpawnParams;
	
	ARogueAICharacter* NewMonster = GetWorld()->SpawnActor<ARogueAICharacter>(SpawnData->MonsterClass.Get(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
}
