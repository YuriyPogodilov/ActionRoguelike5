// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
}

void URogueActionSystemComponent::ApplyHealthChange(float InValueChange)
{
	float OldHealth = Attributes.Health;
	Attributes.Health = FMath::Clamp(Attributes.Health + InValueChange, 0.f, Attributes.MaxHealth);
	
	if (FMath::IsNearlyEqual(OldHealth, Attributes.Health))
	{
		return;
	}
	
	OnHealthChanged.Broadcast(Attributes.Health, OldHealth);
	
	UE_LOG(LogTemp, Log, TEXT("Health changed: %f"), Attributes.Health);
}
