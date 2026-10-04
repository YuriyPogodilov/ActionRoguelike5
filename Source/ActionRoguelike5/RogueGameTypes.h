#pragma once

#include "RogueGameTypes.generated.h"

#define NAME_TargetActor "TargetActor"

#define COLLISION_INTERACTION ECC_GameTraceChannel1
#define COLLISION_PROJECTILE ECC_GameTraceChannel2

class UEnvQuery;
class ARogueAICharacter;

USTRUCT(BlueprintType)
struct FMonsterSpawnData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<ARogueAICharacter> MonsterClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost{ 0.0f };
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
	
	float CurrentCredits{ 0.0f };
	
	float NextTickTime{ 0.0f };
};
