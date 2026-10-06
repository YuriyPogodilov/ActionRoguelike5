// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "RogueAICharacter.generated.h"

class URogueMonsterData;
struct FGameplayTag;
class URogueActionSystemComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE5_API ARogueAICharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	
	UPROPERTY(Transient)
	TObjectPtr<URogueMonsterData> MonsterData;
	
public:
	
	ARogueAICharacter();
	
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	URogueActionSystemComponent* GetActionSystemComponent() const { return ActionSystemComponent; }
	
	URogueMonsterData* GetMonsterData() const { return MonsterData; }
	
	void SetMonsterData(URogueMonsterData* NewMonsterData);
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	
protected:
	
	void OnHealthChanged(FGameplayTag AttributeTag, float NewHealth, float OldHealth);
	
	UFUNCTION()
	void OnGameplayTagsCountUpdate(FGameplayTag UpdatedTag, int32 NewCount);
	
	void HandleKilled();
	
	FTimerHandle OverlayTimerHandle;
	
	bool bIsDead{ false };
};
