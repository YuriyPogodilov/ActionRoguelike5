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

	for (const TPair<FGameplayTag, float>& Cost : ActivationCost)
	{
		GetOwningComponent()->ApplyAttributeChange(Cost.Key, -Cost.Value, Base);
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

	for (const TPair<FGameplayTag, float>& Cost : ActivationCost)
	{
		float AvailableAttributeAmount = GetOwningComponent()->GetAttributeValue(Cost.Key);
		if (AvailableAttributeAmount < Cost.Value)
		{
			UE_LOGFMT(LogTemp, Log, "Not enough {AttributeName} to activate {ActionName}. "
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
