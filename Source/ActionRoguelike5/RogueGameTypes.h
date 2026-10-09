#pragma once

#include "RogueGameTypes.generated.h"

#define NAME_TargetActor "TargetActor"
#define NAME_NextPatrolPoint "NextPatrolPoint"

#define COLLISION_INTERACTION ECC_GameTraceChannel1
#define COLLISION_PROJECTILE ECC_GameTraceChannel2

#define TEAM_ID_PLAYERS 1
#define TEAM_ID_BOTS 2

class URogueMonsterData;
class UEnvQuery;
class ARogueAICharacter;

USTRUCT(BlueprintType)
struct FMonsterSpawnData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<URogueMonsterData> MonsterData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.0f))
	float SpawnCost{ 0.0f };
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.0f))
	float SpawnWeight{ 1.0f };
};

USTRUCT()
struct FRogueDirectorData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	TObjectPtr<UEnvQuery> SpawnLocationQuery;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	TObjectPtr<UDataTable> MonsterSpawnDataTable;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	FRuntimeFloatCurve CreditGainCurve;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	float TickInterval{ 0.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	float TimeBetweenWaves{ 0.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	FString DebugDisplayName{ TEXT("DirectorName") };
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn system")
	FColor DebugColor{ FColor::White };
	
	float CurrentCredits{ 0.0f };
	
	float NextTickTime{ 0.0f };
	
	FRandomStream RandomStream_MonsterSelection;
};
