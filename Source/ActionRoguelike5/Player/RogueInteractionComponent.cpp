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
	
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractionRadius);
	
	AActor* BestActor = nullptr;
	float HighestDot = -1.f;
	
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, CollisionShape);
	
	for (const FOverlapResult& Overlap : Overlaps)
	{
		FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();
		
		float DotResult = FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector());
		if (DotResult > HighestDot)
		{
			BestActor = Overlap.GetActor();
			HighestDot = DotResult;
		}
		
		if (bEnableDebugDraw)
		{
			FString DebugDotValue = FString::Printf(TEXT("%0.3f"), DotResult);
			DrawDebugBox(GetWorld(), OverlapLocation, FVector(40.f), FColor::Red);
			DrawDebugString(GetWorld(), OverlapLocation, DebugDotValue, nullptr, FColor::White, 0, true);
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

