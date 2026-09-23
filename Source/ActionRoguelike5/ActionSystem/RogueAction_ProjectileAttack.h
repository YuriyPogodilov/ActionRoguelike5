// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueAction_ProjectileAttack.generated.h"

class ARogueProjectile;
class UNiagaraSystem;
class USoundBase;


UCLASS(Abstract)
class ACTIONROGUELIKE5_API URogueAction_ProjectileAttack : public URogueAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<ARogueProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> CastingSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	FName MuzzleSocketName{ FName("Muzzle_01") };
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	float AnimationDelay{ 0.f };
	
	void AttackTimerElapsed();
	
public:
	URogueAction_ProjectileAttack();
	
	virtual void StartAction() override;
};
