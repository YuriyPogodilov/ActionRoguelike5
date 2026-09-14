// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueProjectile.generated.h"


class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UAudioComponent;


UCLASS(Abstract)
class ACTIONROGUELIKE5_API ARogueProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> LoopedAudioComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void PlayExplosionEffects();
	
public:
	ARogueProjectile();
	
	virtual void PostInitializeComponents() override;
};
