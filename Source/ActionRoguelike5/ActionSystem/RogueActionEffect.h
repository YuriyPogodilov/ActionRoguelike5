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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Period{ 0.0f };
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Effect")
	int32 StackCount{ 1 };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	bool bResetDurationOnStackIncrease{ true };
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Execute Period Effect", Category="Effect")
	void DynamicExecutePeriodEffect();
	
public:

	virtual void StartAction_Implementation() override;
	
	virtual void StopAction_Implementation() override;
	
	int32 IncrementStackSize();
	
	int32 GetStackCount() const { return StackCount; }
	
protected:
	
	void ResetDuration();
	
	void ExecutePeriodEffect();
	
	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;
};
