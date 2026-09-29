// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "ActionRoguelike5.h"
#include "RogueActionSystemComponent.h"

void URogueAction::StartAction_Implementation()
{
	GetOwningComponent()->AppendActiveTags(GrantTags);

	for (const TPair<FGameplayTag, float>& Cost : ActivationCost)
	{
		GetOwningComponent()->ApplyAttributeChange(Cost.Key, -Cost.Value, Base);
	}
	
	bIsRunning = true;
	
	UE_LOGFMT(LogGame, Log, "Started Action {ActionName} - {WorldTime}", 
		("ActionName", ActionName.ToString()),
		("WorldTime", GetWorld()->TimeSeconds));
}

void URogueAction::StopAction_Implementation()
{
	if (!IsRunning())
	{
		return;
	}
	
	CooldownUntil = GetWorld()->TimeSeconds + CooldownTime;
	
	GetOwningComponent()->RemoveActiveTags(GrantTags);
	
	bIsRunning = false;
	
	UE_LOGFMT(LogGame, Log, "Stopped Action {ActionName} - {WorldTime}", 
		("ActionName", ActionName.ToString()),
		("WorldTime", GetWorld()->TimeSeconds));
}

bool URogueAction::CanStart() const
{
	if (GetCooldownTimeRemaining() > 0.0f)
	{
		UE_LOG(LogGame, Log, TEXT("Cooldown remaining: %f"), GetCooldownTimeRemaining());
		return false;
	}
	
	if (GetOwningComponent()->GetActiveTags().HasAny(BlockedTags))
	{
		return false;
	}

	for (const TPair<FGameplayTag, float>& Cost : ActivationCost)
	{
		float AvailableAttributeAmount = GetOwningComponent()->GetAttributeValue(Cost.Key);
		if (AvailableAttributeAmount < Cost.Value)
		{
			UE_LOGFMT(LogGame, Log, "Not enough {AttributeName} to activate {ActionName}. "
				"Have {AvailableAttributeAmount} and need {RequiredAttributeAmount}",
				("AttributeName", Cost.Key.ToString()),
				("ActionName", ActionName.ToString()),
				("AvailableAttributeAmount", AvailableAttributeAmount),
				("RequiredAttributeAmount", Cost.Value));
			
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
