// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/RogueGameMode.h"
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
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	TArray<FRogueDirectorData> Directors;
	
	void OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> QueryResult, FMonsterSpawnData* SpawnData);
	
	void OnMonsterClassLoaded(const FSoftObjectPath& LoadedObjectPath, UObject* LoadedObject, FVector SpawnLocation, FMonsterSpawnData* SpawnData);
	
	bool TrySpawnMonster(FRogueDirectorData& Director);
	
public:
	
	virtual void Tick(float DeltaSeconds) override;
	
	ARoguePrimaryGameMode();
};
