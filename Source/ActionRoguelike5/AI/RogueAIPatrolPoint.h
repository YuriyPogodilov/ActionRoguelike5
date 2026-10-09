// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueAIPatrolPoint.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE5_API ARogueAIPatrolPoint : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<USceneComponent> DefaultSceneComponent;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Patrol Points")
	TArray<TObjectPtr<ARogueAIPatrolPoint>> NearbyPatrolPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Patrol Points")
	float MaxPathLength{ 1000.0f };
	
public:
	ARogueAIPatrolPoint();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	const TArray<TObjectPtr<ARogueAIPatrolPoint>>& GetNearbyPatrolPoints() const { return NearbyPatrolPoints; }

	void AddPatrolConnection(ARogueAIPatrolPoint* OtherPatrolPoint);
	
	void RemovePatrolConnection(ARogueAIPatrolPoint* OtherPatrolPoint);
};
