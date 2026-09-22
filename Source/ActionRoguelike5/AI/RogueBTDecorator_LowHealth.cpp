// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTDecorator_LowHealth.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"

bool URogueBTDecorator_LowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	
	APawn* OwningPawn = Controller->GetPawn();
	check(OwningPawn);
	
	URogueActionSystemComponent* ActionComp = OwningPawn->GetComponentByClass<URogueActionSystemComponent>();
	check(ActionComp);
	
	FRogueAttributeSet Attributes = ActionComp->GetAttributes();
	
	return (Attributes.Health / Attributes.MaxHealth < LowHealthThreshHold);
}
