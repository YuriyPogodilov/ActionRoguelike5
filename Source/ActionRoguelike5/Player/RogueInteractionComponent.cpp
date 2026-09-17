// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Core/RogueInteractionInterface.h"

TAutoConsoleVariable<bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"), false, 
	TEXT("Enable interaction component debug rendering. (0 = off, 1 = enabled)"),
	ECVF_Cheat);

URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URogueInteractionComponent::Interact() const
{
	if (SelectedActor)
	{
		IRogueInteractionInterface::Execute_Interact(SelectedActor);
	}
}

void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	bool bEnableDebugDraw = CVarInteractionDebugDrawing.GetValueOnGameThread();

	APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	FVector Center = PC->GetPawn()->GetActorLocation();
	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractionRadius);
	
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, CollisionShape);
	
	AActor* BestActor = nullptr;
	float HighestWeight = 0.0f;
	float InteractionRadiusSqrd = InteractionRadius * InteractionRadius;
	
	for (const FOverlapResult& Overlap : Overlaps)
	{
		FVector Origin;
		FVector BoxExtent;
		Overlap.GetActor()->GetActorBounds(true, Origin, BoxExtent);
		
		FVector OverlapDirection = (Origin - CameraLocation).GetSafeNormal();
		
		float DistanceToSqrd = (Origin - Center).SizeSquared();
		float NormalizedDistanceTo = 1.0f - (DistanceToSqrd / InteractionRadiusSqrd);
		
		float DotResult = FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector());
		float NormalizedDotResult = DotResult * 0.5f + 0.5f;
			
		float Weight = (NormalizedDotResult * DirectionWeightScale) + (NormalizedDistanceTo * DistanceToWeightScale);
		if (Weight > HighestWeight)
		{
			BestActor = Overlap.GetActor();
			HighestWeight = Weight;
		}
		
		if (bEnableDebugDraw)
		{
			FString DebugDotValue = FString::Printf(TEXT("Weight: %f, Dot: %f, Dist: %f"), Weight, NormalizedDotResult, NormalizedDistanceTo);
			DrawDebugBox(GetWorld(), Origin, FVector(40.f), FColor::Red);
			DrawDebugString(GetWorld(), Origin, DebugDotValue, nullptr, FColor::White, 0, true);
		}
	}
	
	if (BestActor)
	{
		SelectedActor = BestActor;
	}
	
	if (bEnableDebugDraw)
	{
		if (BestActor)
		{
			DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(50.f), FColor::Green);
		}
		
		DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
	}
}

