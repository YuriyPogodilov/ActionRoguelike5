// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "RogueAIController.generated.h"

class UBehaviorTree;

UCLASS()
class ACTIONROGUELIKE5_API ARogueAIController : public AAIController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	
	virtual void BeginPlay() override;
	
public:
	
	ARogueAIController();
	
	virtual void PreRegisterAllComponents() override;
};
