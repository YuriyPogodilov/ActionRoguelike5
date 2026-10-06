// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "GameFramework/Actor.h"
#include "RogueProjectileMagic.generated.h"


class URogueActionEffect;

UCLASS(Abstract)
class ACTIONROGUELIKE5_API ARogueProjectileMagic : public ARogueProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float AttackDamage{ 51.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float ImpulseIntensity{ 200000.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Noise")
	float NoiseLoudness{ 1.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Noise")
	float NoiseMaxRange{ 100.0f };
	
	UPROPERTY(EditDefaultsOnly, Category="Effect")
	TSubclassOf<URogueActionEffect> ApplyingEffect;
	
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit) override;
	
public:
	
	ARogueProjectileMagic();
};
