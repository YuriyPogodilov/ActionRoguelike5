// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/RogueInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "RogueItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE5_API ARogueItemChest : public AActor, public IRogueInteractionInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> BaseMeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> LidMeshComponent;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	float AnimationSpeed{ 50.f };
	
	UPROPERTY(EditAnywhere, Category="Animation")
	float AnimationTargetPitch{ 120.f };
	
	float CurrentAnimation{ 0.f };
	
	UFUNCTION(BlueprintImplementableEvent)
	void ChestAnimationComplete();

public:
	ARogueItemChest();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation() override;
};
