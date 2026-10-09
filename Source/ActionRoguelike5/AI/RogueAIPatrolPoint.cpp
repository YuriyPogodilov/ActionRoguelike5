// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAIPatrolPoint.h"

#include "ActionRoguelike5.h"
#include "EngineUtils.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"


ARogueAIPatrolPoint::ARogueAIPatrolPoint()
{
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
#if WITH_EDITORONLY_DATA
	DefaultSceneComponent->bVisualizeComponent = true;
#endif
	
	RootComponent = DefaultSceneComponent;
}

void ARogueAIPatrolPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	float DebugDuration = 2.0f;
	const FVector StartLocation = GetActorLocation();
	
	TArray<TWeakObjectPtr<ARogueAIPatrolPoint>> PreviousPatrolPoints(NearbyPatrolPoints);
	TArray<ARogueAIPatrolPoint*> ValidPatrolPoints;
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
	if (!ensure(NavSystem))
	{
		return;
	}
	
	for (ARogueAIPatrolPoint* OtherPatrolPoint : TActorRange<ARogueAIPatrolPoint>(GetWorld()))
	{
		if (OtherPatrolPoint == this)
		{
			continue;
		}
		
		FVector OtherLocation = OtherPatrolPoint->GetActorLocation();
		
		if (FVector::Distance(StartLocation, OtherLocation) > MaxPathLength)
		{
			continue;
		} 
		
		double PathLength = 0.0f;
		ENavigationQueryResult::Type Result = NavSystem->GetPathLength(this, StartLocation, OtherLocation, PathLength);
		if (Result == ENavigationQueryResult::Error)
		{
			UE_LOG(LogGame, Warning, TEXT("Unable to make patrol points, nav data not ready."));
			return;
		}
		
		if (Result == ENavigationQueryResult::Success && PathLength < MaxPathLength)
		{
			ValidPatrolPoints.Add(OtherPatrolPoint);
			
			OtherPatrolPoint->AddPatrolConnection(this);
			
			DrawDebugBox(GetWorld(), OtherLocation, FVector(20.0f), FColor::Green, false, DebugDuration);
			DrawDebugDirectionalArrow(GetWorld(), StartLocation, OtherLocation, 64.0f, FColor::Green, false, DebugDuration);
		}
	}
	
	NearbyPatrolPoints = MoveTemp(ValidPatrolPoints);

	for (TWeakObjectPtr<ARogueAIPatrolPoint> OldPatrolPoint : PreviousPatrolPoints)
	{
		if (!NearbyPatrolPoints.Contains(OldPatrolPoint))
		{
			OldPatrolPoint->RemovePatrolConnection(this);
			
			FVector OtherLocation = OldPatrolPoint->GetActorLocation();
			DrawDebugBox(GetWorld(), OtherLocation, FVector(20.0f), FColor::Orange, false, DebugDuration);
			DrawDebugDirectionalArrow(GetWorld(), StartLocation, OtherLocation, 64.0f, FColor::Orange, false, DebugDuration);
		}
	}
}

void ARogueAIPatrolPoint::AddPatrolConnection(ARogueAIPatrolPoint* OtherPatrolPoint)
{
	if (!NearbyPatrolPoints.Contains(OtherPatrolPoint))
	{
		NearbyPatrolPoints.Add(OtherPatrolPoint);
		MarkPackageDirty();	
	}
}

void ARogueAIPatrolPoint::RemovePatrolConnection(ARogueAIPatrolPoint* OtherPatrolPoint)
{
	int32 RemoveCount = NearbyPatrolPoints.RemoveSingleSwap(OtherPatrolPoint);
	if (RemoveCount > 0)
	{
		MarkPackageDirty();
	}
}
