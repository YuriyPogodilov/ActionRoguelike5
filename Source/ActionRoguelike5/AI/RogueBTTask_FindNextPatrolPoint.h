// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RogueBTTask_FindNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE5_API URogueBTTask_FindNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector NextPatrolPointKey;

	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector PreviousPatrolPointKey;
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
