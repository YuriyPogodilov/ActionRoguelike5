// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionEffect.h"

#include "ActionRoguelike5.h"
#include "RogueActionSystemComponent.h"

void URogueActionEffect::StartAction_Implementation()
{
	Super::StartAction_Implementation();
	
	if (Duration > 0.0f)
	{
		ResetDuration();
	}
	
	if (Period > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, this, &ThisClass::ExecutePeriodEffect, Period);
	}
}

void URogueActionEffect::StopAction_Implementation()
{
	if (Period > 0.0f && GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect();
	}
	
	Super::StopAction_Implementation();
	
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	
	GetOwningComponent()->RemoveAction(this);
}

int32 URogueActionEffect::IncrementStackSize()
{
	++StackCount;
	
	GetOwningComponent()->AppendActiveTags(GrantTags);
	
	if (bResetDurationOnStackIncrease)
	{
		ResetDuration();
	}
	
	UE_LOG(LogGame, Log, TEXT("Incremented %s (%s) Stack to %d"),
		*GetName(),
		*GetNameSafe(GetOwningComponent()->GetOwner()),
		StackCount);
	
	return StackCount;
}

void URogueActionEffect::ResetDuration()
{
	GetWorld()->GetTimerManager().SetTimer(DurationHandle, this, &ThisClass::StopAction, Duration);
}

void URogueActionEffect::ExecutePeriodEffect()
{
	DynamicExecutePeriodEffect();
}
