// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueTeleportProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE5_API ARogueTeleportProjectile : public ARogueProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float DetonateDelay{ 0.2f };
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float TeleportationDelay{ 0.1f };
	
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void BeginPlay() override;
	
	void StartDelayedTeleport();
	void HandleTeleportation();
	
public:
	ARogueTeleportProjectile();
};
