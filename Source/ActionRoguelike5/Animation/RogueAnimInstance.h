// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RogueAnimInstance.generated.h"

class URogueActionSystemComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE5_API URogueAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="StatusEffect")
	bool bIsSprinting{ false };
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<URogueActionSystemComponent> ActionComp;

public:
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
