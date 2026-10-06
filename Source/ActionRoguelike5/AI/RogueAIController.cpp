// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAIController.h"

#include "RogueGameTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


ARogueAIController::ARogueAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
}

void ARogueAIController::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();
	
	// Need to be super early before pawn is registered or perception system might have the wrong teamId
	SetGenericTeamId(FGenericTeamId(TEAM_ID_BOTS));
}

void ARogueAIController::BeginPlay()
{
	Super::BeginPlay();
	
	RunBehaviorTree(BehaviorTree);
}
