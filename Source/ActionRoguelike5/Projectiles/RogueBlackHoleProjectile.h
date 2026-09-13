// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueBlackHoleProjectile.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE5_API ARogueBlackHoleProjectile : public ARogueProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability Parameters")
	float Duration{ 5.f };
	
	virtual void BeginPlay() override;
	
	void TimerElapsed();
	
	UFUNCTION()
	void OnActorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
public:
	ARogueBlackHoleProjectile();
	
	virtual void PostInitializeComponents() override;
};
