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

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayTag /*AttributeTag*/, float /*NewHealth*/, float /*OldHealth*/);

UENUM(BlueprintType)
enum EAttributeModifyType
{
	Base,
	Modifier,
	OverrideBase,
	Invalid
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE5_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY()
	TObjectPtr<URogueAttributeSet> Attributes;
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes", NoClear)
	TSubclassOf<URogueAttributeSet> AttributesSetClass;
	
	TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;
	
	TMap<FGameplayTag, FOnAttributeChanged> AttributeListeners;
	
	UPROPERTY()	
	TArray<TObjectPtr<URogueAction>> Actions;
	
	UPROPERTY(EditAnywhere, Category="Actions")	
	TArray<TSubclassOf<URogueAction>> DefaultActions;
	
public:
	
	FGameplayTagContainer ActiveGameplayTags;
	
	URogueActionSystemComponent();
	
	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;
	
	
	void GrantAction(TSubclassOf<URogueAction> NewActionClass);

	void StartAction(FGameplayTag InActionName);
	
	void StopAction(FGameplayTag InActionName);
	
	
	UFUNCTION(BlueprintCallable)
	void ApplyAttributeChange(FGameplayTag AttributeTag, float InValueChange, EAttributeModifyType ModifyType);
	
	FRogueAttribute* GetAttribute(FGameplayTag InAttributeTag) const;
	
	FOnAttributeChanged& GetAttributeListener(FGameplayTag AttributeTag);
};
