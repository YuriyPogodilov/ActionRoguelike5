// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTTask_FindNextPatrolPoint.h"

#include "ActionRoguelike5.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "RogueAIPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URogueBTTask_FindNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	check(BBComp);
	
	AActor* NextPatrolPoint = nullptr;

	ARogueAIPatrolPoint* CurrentPatrolPoint = Cast<ARogueAIPatrolPoint>(BBComp->GetValueAsObject(NextPatrolPointKey.SelectedKeyName));
	if (!IsValid(CurrentPatrolPoint)) {
		AAIController* Controller = OwnerComp.GetAIOwner();
	
		APawn* OwningPawn = Controller->GetPawn();
		check(OwningPawn);
		
		FVector OwnerLocation = OwningPawn->GetActorLocation();
	
		ARogueAIPatrolPoint* NearestPatrolPoint = nullptr;
		float NearestDistance = MAX_FLT;

		for (ARogueAIPatrolPoint* Point : TActorRange<ARogueAIPatrolPoint>(OwnerComp.GetWorld()))
		{
			float Distance = FVector::Dist(OwnerLocation, Point->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestPatrolPoint = Point;
				NearestDistance = Distance;
			}
		}
		
		NextPatrolPoint = NearestPatrolPoint;

		if (NextPatrolPoint == nullptr) {
			UE_LOG(LogGame, Warning, TEXT("No patrol point available for %s"), *GetNameSafe(OwnerComp.GetOwner()));
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		ARogueAIPatrolPoint* PreviousPatrolPoint = Cast<ARogueAIPatrolPoint>(BBComp->GetValueAsObject(PreviousPatrolPointKey.SelectedKeyName));
		BBComp->SetValueAsObject(PreviousPatrolPointKey.SelectedKeyName, CurrentPatrolPoint);

		TArray<ARogueAIPatrolPoint*> Connections = CurrentPatrolPoint->GetNearbyPatrolPoints();
		if (Connections.IsEmpty())
		{
			UE_LOG(LogGame, Warning, TEXT("PatrolPoint %s has no connections available for %s"), 
				*GetNameSafe(CurrentPatrolPoint),
				*GetNameSafe(OwnerComp.GetOwner()));
			
			return EBTNodeResult::Failed;
		}
		
		if (Connections.Num() > 1)
		{
			Connections.RemoveSingle(PreviousPatrolPoint);
		}
		
		int32 RandIndex = FMath::RandRange(0, Connections.Num() - 1);
		NextPatrolPoint = Connections[RandIndex];
	}
	
	BBComp->SetValueAsObject(NextPatrolPointKey.SelectedKeyName, NextPatrolPoint);
	
	return EBTNodeResult::Succeeded;
}
