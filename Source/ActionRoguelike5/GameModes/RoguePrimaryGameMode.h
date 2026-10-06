// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueGameMode.h"
#include "RoguePrimaryGameMode.generated.h"

struct FRogueDirectorData;
struct FMonsterSpawnData;
struct FEnvQueryResult;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE5_API ARoguePrimaryGameMode : public ARogueGameMode
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system", meta=(TitleProperty="DebugDisplayName"))
	TArray<FRogueDirectorData> Directors;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	int32 GlobalStartingSeed{ 0 };
	
	void OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> QueryResult, FMonsterSpawnData* SpawnData);
	
	void OnMonsterClassLoaded(const FSoftObjectPath& LoadedObjectPath, UObject* LoadedObject, FVector SpawnLocation, FMonsterSpawnData* SpawnData);
	
	bool TrySpawnMonster(FRogueDirectorData& Director);
	
	void SpawnCoinsAtLocation(const FVector& LocationToSpawnAt, int32 CreditsAmount);
	
public:

	virtual void StartPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	ARoguePrimaryGameMode();
};
