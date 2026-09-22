// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BTService.h"
#include "RogueBTService_CheckRangeTo.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE5_API URogueBTService_CheckRangeTo : public UBTService
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector WithinRangeKey;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float MaxAttackRange;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
public:
	URogueBTService_CheckRangeTo();
};
