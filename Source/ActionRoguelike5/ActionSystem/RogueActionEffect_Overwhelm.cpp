// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionEffect_Overwhelm.h"

#include "ActionRoguelike5.h"
#include "RogueActionSystemComponent.h"
#include "SharedGameplayTags.h"

URogueActionEffect_Overwhelm::URogueActionEffect_Overwhelm()
{
	ActionName = SharedGameplayTags::StatusEffect_Overwhelm;
	
	GrantTags.AddTag(SharedGameplayTags::StatusEffect_Overwhelm);
}

void URogueActionEffect_Overwhelm::StartAction_Implementation()
{
	Super::StartAction_Implementation();
	
	GetOwningComponent()->GetOwner()->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnOwnerTakeDamage);
}

void URogueActionEffect_Overwhelm::StopAction_Implementation()
{
	Super::StopAction_Implementation();
	
	GetOwningComponent()->GetOwner()->OnTakeAnyDamage.RemoveAll(this);
}

void URogueActionEffect_Overwhelm::OnOwnerTakeDamage(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (GetOwningComponent()->GetActiveTags().HasTag(SharedGameplayTags::StatusEffect_Stunned))
	{
		AccumulatedDamage = 0;
		MostRecentDamageTime = CurrentTime;
		
		return;
	}
	
	if (CurrentTime - MostRecentDamageTime < OverwhelmEffectFrequency)
	{
		AccumulatedDamage += Damage;
	}
	else
	{
		AccumulatedDamage = Damage;
	}
	
	MostRecentDamageTime = CurrentTime;
	
	if (AccumulatedDamage > OverwhelmEffectDmgThreshold)
	{
		AccumulatedDamage = 0;
		
		check(IsValid(StunEffect));
		
		GetOwningComponent()->GrantAction(StunEffect);
		
		UE_LOG(LogGame, Log, TEXT("Stun applied to %s"), *GetNameSafe(GetOwningComponent()->GetOwner()));
	}
}
