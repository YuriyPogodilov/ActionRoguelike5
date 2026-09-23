// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

#include "RogueAction.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	bWantsInitializeComponent = true;
}

void URogueActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (TSubclassOf<URogueAction> ActionClass : DefaultActions)
	{
		if (ensure(ActionClass))
		{
			GrantAction(ActionClass);
		}
	}
}

void URogueActionSystemComponent::GrantAction(TSubclassOf<URogueAction> NewActionClass)
{
	URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
	Actions.Add(NewAction);
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

void URogueActionSystemComponent::StartAction(FName InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action->GetActionName() == InActionName)
		{
			Action->StartAction();
			return;
		}
	}
	
	UE_LOGFMT(LogTemp, Warning, "No Action found with name {ActionName}", InActionName);
}

const FRogueAttributeSet& URogueActionSystemComponent::GetAttributes() const
{
	return Attributes;
}
