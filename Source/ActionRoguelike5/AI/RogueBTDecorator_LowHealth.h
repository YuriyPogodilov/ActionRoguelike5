// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RogueBTDecorator_LowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE5_API URogueBTDecorator_LowHealth : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, Category="AI")
	float LowHealthThreshHold { 0.3f };
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
