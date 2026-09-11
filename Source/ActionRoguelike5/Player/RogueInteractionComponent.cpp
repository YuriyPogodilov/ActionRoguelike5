// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	FVector Center = PC->GetPawn()->GetActorLocation();
	
	ECollisionChannel CollisionChannel = ECC_Visibility;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractionRadius);
	
	DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
	
	AActor* BestActor = nullptr;
	float HighestDot = -1.f;
	
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, CollisionShape);
	
	for (const FOverlapResult& Overlap : Overlaps)
	{
		FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		
		DrawDebugBox(GetWorld(), OverlapLocation, FVector(40.f), FColor::Red);
		
		FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();
		
		float DotResult = FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector());
		
		FString DebugDotValue = FString::Printf(TEXT("%0.3f"), DotResult);
		DrawDebugString(GetWorld(), OverlapLocation, DebugDotValue, nullptr, FColor::White, 0, true);
		
		if (DotResult > HighestDot)
		{
			BestActor = Overlap.GetActor();
			HighestDot = DotResult;
		}
	}
	
	if (BestActor)
	{
		DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(50.f), FColor::Green);
	}
}

