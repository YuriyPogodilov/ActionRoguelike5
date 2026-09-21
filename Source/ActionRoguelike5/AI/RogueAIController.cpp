// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAIController.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


ARogueAIController::ARogueAIController()
{
}

void ARogueAIController::BeginPlay()
{
	Super::BeginPlay();
	
	RunBehaviorTree(BehaviorTree);
	
	FName TargetActor = "TargetActor";
	
	AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	check(PlayerPawn);
	
	GetBlackboardComponent()->SetValueAsObject(TargetActor, PlayerPawn);
}
