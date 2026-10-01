// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueActionEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE5_API URogueActionEffect : public URogueAction
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Effect")
	float Duration{ 0.0f };
	
public:

	virtual void StartAction_Implementation() override;
	
	virtual void StopAction_Implementation() override;
	
protected:
	
	FTimerHandle DurationHandle;
};
