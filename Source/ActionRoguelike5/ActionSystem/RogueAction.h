// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RogueAction.generated.h"

class URogueActionSystemComponent;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class ACTIONROGUELIKE5_API URogueAction : public UObject
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	FName ActionName { "PrimaryAttack" };
	
public:
	
	UFUNCTION()
	virtual void StartAction();
	
	URogueActionSystemComponent* GetOwningComponent() const;
	
	FName GetActionName() const { return ActionName; }
};
