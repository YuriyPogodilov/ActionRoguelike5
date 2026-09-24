// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"


struct FGameplayTag;
class URogueAction;

USTRUCT(BlueprintType)
struct FRogueAttributeSet
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float MaxHealth{ 100.f };
	
	UPROPERTY(BlueprintReadOnly)
	float Health;
	
	FRogueAttributeSet()
		: Health(MaxHealth)
	{}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, OldHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE5_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FRogueAttributeSet Attributes;
	
	UPROPERTY()	
	TArray<TObjectPtr<URogueAction>> Actions;
	
	UPROPERTY(EditAnywhere, Category="Actions")	
	TArray<TSubclassOf<URogueAction>> DefaultActions;
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	FGameplayTagContainer ActiveGameplayTags;
	
	URogueActionSystemComponent();
	
	virtual void InitializeComponent() override;
	
	void GrantAction(TSubclassOf<URogueAction> NewActionClass);

	UFUNCTION(BlueprintCallable)
	void ApplyHealthChange(float InValueChange);
	
	void StartAction(FGameplayTag InActionName);
	
	void StopAction(FGameplayTag InActionName);
	
	const FRogueAttributeSet& GetAttributes() const;
};
