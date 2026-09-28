// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTDecorator_LowHealth.h"

#include "AIController.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"

bool URogueBTDecorator_LowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	
	APawn* OwningPawn = Controller->GetPawn();
	check(OwningPawn);
	
	URogueActionSystemComponent* ActionComp = OwningPawn->GetComponentByClass<URogueActionSystemComponent>();
	check(ActionComp);
	
	float Health = ActionComp->GetAttributeValue(SharedGameplayTags::Attribute_Health);
	float HealthMax = ActionComp->GetAttributeValue(SharedGameplayTags::Attribute_HealthMax);
	
	return (Health / HealthMax) < LowHealthThreshHold;
}
