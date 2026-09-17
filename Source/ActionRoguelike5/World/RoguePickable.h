// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoguePickable.generated.h"

class USphereComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE5_API ARoguePickable : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<USphereComponent> OverlapComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> PickUpSound;
	
	UFUNCTION()
	void OnActorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	void PickUp(AActor* OtherActor);
	
	UFUNCTION(BlueprintNativeEvent)
	bool ApplyPickupEffect(AActor* OtherActor);
	
	virtual bool ApplyPickupEffect_Implementation(AActor* OtherActor);
	
public:
	ARoguePickable();
	
	virtual void PostInitializeComponents() override;
};
