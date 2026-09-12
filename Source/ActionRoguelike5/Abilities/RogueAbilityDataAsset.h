#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RogueAbilityDataAsset.generated.h"

class UNiagaraSystem;
class ARogueProjectile;

USTRUCT(BlueprintType)
struct FRogueAbilityData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<ARogueProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> CastingSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	FName MuzzleSocketName{ FName("Muzzle_O1") };
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	float AnimationDelay{ 0.f };
};

UCLASS(BlueprintType)
class ACTIONROGUELIKE5_API URogueAbilityDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityData")
	FRogueAbilityData AbilityData;
};
