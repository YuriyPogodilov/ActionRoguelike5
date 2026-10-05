// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueAICharacter.generated.h"

class URogueMonsterData;
struct FGameplayTag;
class URogueActionSystemComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE5_API ARogueAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Death")
	TObjectPtr<UAnimMontage> DeathMontage;
	
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
	
protected:
	
	void OnHealthChanged(FGameplayTag AttributeTag, float NewHealth, float OldHealth);
	
	UFUNCTION()
	void OnGameplayTagsCountUpdate(FGameplayTag UpdatedTag, int32 NewCount);
	
	FTimerHandle OverlayTimerHandle;
};
