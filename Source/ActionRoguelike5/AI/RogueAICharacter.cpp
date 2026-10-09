// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "RogueAIController.h"
#include "RogueMonsterData.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "ActionSystem/RogueAttributeSet.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/RogueGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Perception/AISense_Damage.h"
#include "Pickups/RogueCoinPickupSubsystem.h"


ARogueAICharacter::ARogueAICharacter()
{
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp"));
	ActionSystemComponent->SetDefaultAttributeSet(URogueMonsterAttributeSet::StaticClass());
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARogueAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetMesh()->SetOverlayMaterialMaxDrawDistance(1.0f);
	
	ActionSystemComponent->GetAttributeListener(SharedGameplayTags::Attribute_Health).AddUObject(this, &ThisClass::OnHealthChanged);
	
	ActionSystemComponent->OnGameplayTagsCountUpdate.AddDynamic(this, &ThisClass::OnGameplayTagsCountUpdate);
}

void ARogueAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	URogueGameInstance* GI = GetGameInstance<URogueGameInstance>();
	check(GI);
	GI->AliveMonsters.Add(this);
}

void ARogueAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	URogueGameInstance* GI = GetGameInstance<URogueGameInstance>();
	GI->AliveMonsters.RemoveSingleSwap(this, EAllowShrinking::No);
}

void ARogueAICharacter::OnGameplayTagsCountUpdate(FGameplayTag UpdatedTag, int32 NewCount)
{
	if (UpdatedTag.MatchesTag(SharedGameplayTags::StatusEffect_Stunned))
	{
		ARogueAIController* AIController = CastChecked<ARogueAIController>(GetController());
		UBehaviorTreeComponent* BehaviorTreeComp = AIController->GetComponentByClass<UBehaviorTreeComponent>();
		
		check(BehaviorTreeComp);
		
		if (NewCount > 0)
		{
			BehaviorTreeComp->PauseLogic(TEXT("Got stunned"));
			
			GetCharacterMovement()->SetMovementMode(MOVE_None);
		}
		else
		{
			BehaviorTreeComp->ResumeLogic(TEXT("Stun effect ended"));
			
			GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
		}
	}
}

void ARogueAICharacter::HandleKilled()
{
	if (bIsDead)
	{
		return;
	}
	
	bIsDead = true;
	
	URogueGameInstance* GI = GetGameInstance<URogueGameInstance>();
	GI->AliveMonsters.RemoveSingleSwap(this, EAllowShrinking::No);
	
	ARogueAIController* AIController = CastChecked<ARogueAIController>(GetController());
	AIController->GetBrainComponent()->StopLogic("Killed");
	
	GetMesh()->bPauseAnims = true;
	
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SetLifeSpan(10.0f);
	
	int32 CreditsToReward = 10;
	if (MonsterData && MonsterData->CreditsReward > 0)
	{
		CreditsToReward = MonsterData->CreditsReward;
	}
	
	URogueCoinPickupSubsystem* CoinPickupSubsystem = GetWorld()->GetSubsystem<URogueCoinPickupSubsystem>();
	CoinPickupSubsystem->SpawnCoinsAtLocation(GetActorLocation(), CreditsToReward);
}

void ARogueAICharacter::OnHealthChanged(FGameplayTag AttributeTag, float NewHealth, float OldHealth)
{
	if (FMath::IsNearlyZero(NewHealth))
	{
		HandleKilled();
	}
}

float ARogueAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (IsValid(EventInstigator))
	{
		if (GetTeamAttitudeTowards(*EventInstigator) == ETeamAttitude::Hostile)
		{
			UAISense_Damage::ReportDamageEvent(this, this, EventInstigator->GetPawn(), 
				ActualDamage, EventInstigator->GetPawn()->GetActorLocation(), GetActorLocation());
		}
	}
	
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -DamageAmount, EAttributeModifyType::Base);
	
	// HitFlash
	GetMesh()->SetOverlayMaterialMaxDrawDistance(0.0f);
	
	GetMesh()->SetCustomPrimitiveDataFloat(0, GetWorld()->GetTimeSeconds());
	
	GetWorldTimerManager().SetTimer(OverlayTimerHandle, [this]()
	{
		GetMesh()->SetOverlayMaterialMaxDrawDistance(1.0f);
	}, 1.0f, false);
	
	return ActualDamage;
}

void ARogueAICharacter::SetMonsterData(URogueMonsterData* NewMonsterData)
{
	check(MonsterData == nullptr);
	MonsterData = NewMonsterData;
}

FGenericTeamId ARogueAICharacter::GetGenericTeamId() const
{
	if (AAIController* AIC = GetController<AAIController>())
	{
		return AIC->GetGenericTeamId();
	}
	
	return FGenericTeamId::NoTeam;
}

