// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	HealthMax = FRogueAttribute(100.0f);
	Health = FRogueAttribute(HealthMax.GetValue());
}
