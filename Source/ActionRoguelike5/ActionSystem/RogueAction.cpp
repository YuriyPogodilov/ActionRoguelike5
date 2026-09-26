// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "RogueActionSystemComponent.h"
#include "SharedGameplayTags.h"

void URogueAction::StartAction_Implementation()
{
	bIsRunning = true;
	
	float GameTime = GetWorld()->TimeSeconds;
	
	UE_LOGFMT(LogTemp, Log, "Started Action {ActionName} - {WorldTime}", 
		("ActionName", ActionName.ToString()),
		("WorldTime", GameTime));
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantTags);
	
	if (!FMath::IsNearlyZero(RageCost))
	{
		GetOwningComponent()->ApplyAttributeChange(SharedGameplayTags::Attribute_Rage, -RageCost, Base);
	}
}

void URogueAction::StopAction_Implementation()
{
	if (!IsRunning())
	{
		return;
	}
	
	bIsRunning = false;
	
	float GameTime = GetWorld()->TimeSeconds;
	
	UE_LOGFMT(LogTemp, Log, "Stopped Action {ActionName} - {WorldTime}", 
		("ActionName", ActionName.ToString()),
		("WorldTime", GameTime));
	
	CooldownUntil = GetWorld()->TimeSeconds + CooldownTime;
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantTags);
}

bool URogueAction::CanStart() const
{
	if (GetCooldownTimeRemaining() > 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Cooldown remaining: %f"), GetCooldownTimeRemaining());
		return false;
	}
	
	if (GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	if (!FMath::IsNearlyZero(RageCost))
	{
		float RageAmount = GetOwningComponent()->GetAttributeValue(SharedGameplayTags::Attribute_Rage);
		if (RageAmount < RageCost)
		{
			UE_LOG(LogTemp, Log, TEXT("Not enough rage."));
			return false;
		}
	}
	
	return true;
}

URogueActionSystemComponent* URogueAction::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter()); 
}

float URogueAction::GetCooldownTimeRemaining() const
{
	return FMath::Max(0.0f, CooldownUntil - GetWorld()->TimeSeconds);
}
