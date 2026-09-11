// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueItemChest.h"


ARogueItemChest::ARogueItemChest()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	BaseMeshComponent->SetCollisionProfileName("Interaction");
	RootComponent = BaseMeshComponent;
	
	LidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	LidMeshComponent->SetCollisionProfileName("NoCollision");
	LidMeshComponent->SetupAttachment(BaseMeshComponent);
}

void ARogueItemChest::Interact_Implementation()
{
	SetActorTickEnabled(true);
}

void ARogueItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentAnimation = FMath::FInterpConstantTo(CurrentAnimation, AnimationTargetPitch, DeltaTime, AnimationSpeed);
	
	LidMeshComponent->SetRelativeRotation(FRotator(CurrentAnimation, 0.f, 0.f));
	
	if (FMath::IsNearlyEqual(CurrentAnimation, AnimationTargetPitch))
	{
		ChestAnimationComplete();
		
		SetActorTickEnabled(false);
	}
}
