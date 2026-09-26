// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

#include "RogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


/// -------------------- RogueAttributeSet ---------------------- 
URogueActionSystemComponent* URogueAttributeSet::GetOwningComponent() const
{
	return CastChecked<URogueActionSystemComponent>(GetOuter());
}

/// -------------------- RogueHealthAttributeSet ---------------------- 
URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	HealthMax = FRogueAttribute(100.0f);
	Health = FRogueAttribute(HealthMax.GetValue());
}

void URogueHealthAttributeSet::PostAttributeChanged()
{
	Super::PostAttributeChanged();
	
	Health.Base = FMath::Clamp(Health.Base, 0.0f, HealthMax.GetValue());
}

/// -------------------- RoguePawnAttributeSet ---------------------- 
URoguePawnAttributeSet::URoguePawnAttributeSet()
{
	MoveSpeed = FRogueAttribute(550.0f);
	MoveSpeedMultiplier = FRogueAttribute(1.0f);
}

void URoguePawnAttributeSet::ApplyMoveSpeed()
{
	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.GetValue() * MoveSpeedMultiplier.GetValue();
}

void URoguePawnAttributeSet::InitializeAttribute()
{
	Super::InitializeAttribute();
	
	ApplyMoveSpeed();
}

void URoguePawnAttributeSet::PostAttributeChanged() {
	Super::PostAttributeChanged();
	
	ApplyMoveSpeed();
}

/// -------------------- RoguePlayerAttributeSet ---------------------- 
URoguePlayerAttributeSet::URoguePlayerAttributeSet()
{
	Rage = FRogueAttribute(0.0f);
	RageMax = FRogueAttribute(100.0f);
}

void URoguePlayerAttributeSet::PostAttributeChanged()
{
	Super::PostAttributeChanged();
	
	Rage.Base = FMath::Clamp(Rage.GetValue(), 0.0f, RageMax.GetValue());
}

/// -------------------- RogueMonsterAttributeSet ---------------------- 
URogueMonsterAttributeSet::URogueMonsterAttributeSet()
{
	MoveSpeed = FRogueAttribute(450.0f);
}
