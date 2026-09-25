// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	HealthMax = FRogueAttribute(100.0f);
	Health = FRogueAttribute(HealthMax.GetValue());
}

void URogueHealthAttributeSet::PostAttributeChanged()
{
	Health.Base = FMath::Clamp(Health.Base, 0.0f, HealthMax.GetValue());
}
