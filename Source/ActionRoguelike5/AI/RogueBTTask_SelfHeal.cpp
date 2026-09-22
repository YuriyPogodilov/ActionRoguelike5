// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTTask_SelfHeal.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"

EBTNodeResult::Type URogueBTTask_SelfHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	
	APawn* OwningPawn = Controller->GetPawn();
	check(OwningPawn);
	
	URogueActionSystemComponent* ActionComp = OwningPawn->GetComponentByClass<URogueActionSystemComponent>();
	check(ActionComp);

	ActionComp->ApplyHealthChange(HealAmount);
	
	return EBTNodeResult::Succeeded;
}
