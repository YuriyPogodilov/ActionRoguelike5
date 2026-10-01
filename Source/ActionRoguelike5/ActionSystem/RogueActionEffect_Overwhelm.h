// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueActionEffect.h"
#include "RogueActionEffect_Overwhelm.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE5_API URogueActionEffect_Overwhelm : public URogueActionEffect
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Overwhelm")
	float OverwhelmEffectDmgThreshold{ 50.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Overwhelm")
	float OverwhelmEffectFrequency{ 1.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Overwhelm")
	TSubclassOf<URogueActionEffect> StunEffect;
	
public:
	
	URogueActionEffect_Overwhelm();
	
	virtual void StartAction_Implementation() override;

	virtual void StopAction_Implementation() override;
	
protected:
	
	UFUNCTION()
	void OnOwnerTakeDamage(AActor* DamagedActor, float Damage, 
		const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	float MostRecentDamageTime{ -10.0f };
	
	float AccumulatedDamage{ 0.0f };
};
