// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/PawnMovementComponent.h"


void ARogueAICharacter::OnHealthChanged(FGameplayTag AttributeTag, float NewHealth, float OldHealth)
{
	if (FMath::IsNearlyZero(NewHealth))
	{
		DetachFromControllerPendingDestroy();
		
		GetMovementComponent()->StopMovementImmediately();
		
		PlayAnimMontage(DeathMontage);
	}
}

ARogueAICharacter::ARogueAICharacter()
{
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp"));
}

void ARogueAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ActionSystemComponent->GetAttributeListener(SharedGameplayTags::Attribute_Health).AddUObject(this, &ThisClass::OnHealthChanged);
}

float ARogueAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -DamageAmount, EAttributeModifyType::Base);
	
	return ActualDamage;
}

