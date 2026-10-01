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

void URogueActionEffect_Overwhelm::OnOwnerTakeDamage(AActor* DamagedActor, float Damage,
                                                     const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (GetOwningComponent()->GetActiveTags().HasTag(SharedGameplayTags::StatusEffect_Stunned))
	{
		AccumulatedDamage = 0;
		MostRecentDamageTime = CurrentTime;
		
		UE_LOG(LogGame, Warning, TEXT("Minion already stunned. Reset Accumulated dmg"));
		
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
	
	UE_LOG(LogGame, Warning, TEXT("Received dmg: %.2f. Accumulated dmg: %.2f"), Damage, AccumulatedDamage);
	
	if (AccumulatedDamage > OverwhelmEffectDmgThreshold)
	{
		AccumulatedDamage = 0;
		
		check(IsValid(StunEffect));
		
		GetOwningComponent()->GrantAction(StunEffect);
		
		UE_LOG(LogGame, Warning, TEXT("Minion got stunned"));
	}
}
