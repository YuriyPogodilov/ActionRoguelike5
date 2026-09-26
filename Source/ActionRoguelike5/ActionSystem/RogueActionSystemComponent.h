// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"


class URogueAttributeSet;
struct FRogueAttribute;
struct FGameplayTag;
class URogueAction;

// Native C++
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayTag /*AttributeTag*/, float /*NewHealth*/, float /*OldHealth*/);
// Blueprint delegate
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnAttributeChanged_Dynamic, FGameplayTag, AttributeTag, float, NewHealth, float, OldHealth);

UENUM(BlueprintType)
enum EAttributeModifyType
{
	Base,
	Modifier,
	OverrideBase,
	Invalid
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=(Navigation,Cooking,Tags))
class ACTIONROGUELIKE5_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, Instanced, NoClear, Category="ActionSystem")
	TObjectPtr<URogueAttributeSet> Attributes;
	
	TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;
	
	TMap<FGameplayTag, FOnAttributeChanged> AttributeListeners;
	
	TMap<FGameplayTag, TArray<FOnAttributeChanged_Dynamic>> AttributeDynamicListeners;
	
	UPROPERTY()	
	TArray<TObjectPtr<URogueAction>> Actions;
	
	UPROPERTY(EditAnywhere, Category="ActionSystem")	
	TArray<TSubclassOf<URogueAction>> DefaultActions;
	
public:
	
	FGameplayTagContainer ActiveGameplayTags;
	
	URogueActionSystemComponent();
	
	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;
	
	
	void GrantAction(TSubclassOf<URogueAction> NewActionClass);

	void StartAction(FGameplayTag InActionName);
	
	void StopAction(FGameplayTag InActionName);
	
	
	void SetDefaultAttributeSet(TSubclassOf<URogueAttributeSet> AttributeClass);
	
	UFUNCTION(BlueprintCallable)
	void ApplyAttributeChange(FGameplayTag AttributeTag, float InValueChange, EAttributeModifyType ModifyType);
	
	FRogueAttribute* GetAttribute(FGameplayTag AttributeTag) const;
	
	UFUNCTION(BlueprintCallable)
	float GetAttributeValue(FGameplayTag AttributerTag) const;
	
	FOnAttributeChanged& GetAttributeListener(FGameplayTag AttributeTag);
	
	UFUNCTION(BlueprintCallable, DisplayName="Add Attribute Listener", meta=(Keywords="delegate,event"))
	void AddDynamicAttributeListener(FOnAttributeChanged_Dynamic Event, FGameplayTag AttributeTag);
	
	UFUNCTION(BlueprintCallable, DisplayName="Remove Attribute Listener", meta=(Keywords="delegate,event"))
	void RemoveDynamicAttributeListener(FOnAttributeChanged_Dynamic Event);
};
