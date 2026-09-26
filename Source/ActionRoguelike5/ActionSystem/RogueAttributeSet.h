// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RogueAttributeSet.generated.h"

class URogueActionSystemComponent;

USTRUCT()
struct FRogueAttribute
{
	GENERATED_BODY()
	
	FRogueAttribute() { }
	
	explicit FRogueAttribute(float InBase) 
		: Base(InBase) {}
	
	UPROPERTY(EditAnywhere)
	float Base{ 0.0f };
	
	UPROPERTY(Transient)
	float Modifier{ 0.0f };
	
	float GetValue() const
	{
		return Base + Modifier;
	}
};

UCLASS(EditInlineNew)
class ACTIONROGUELIKE5_API URogueAttributeSet : public UObject
{
	GENERATED_BODY()
	
public:
	
	URogueActionSystemComponent* GetOwningComponent() const;
	
	virtual void InitializeAttribute() {}
	
	virtual void PostAttributeChanged() {}
};

UCLASS()
class URogueHealthAttributeSet : public URogueAttributeSet
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category="Health")
	FRogueAttribute Health;
	
	UPROPERTY(EditAnywhere, Category="Health")
	FRogueAttribute HealthMax;
	
	virtual void PostAttributeChanged() override;
	
	URogueHealthAttributeSet();
};

UCLASS()
class URoguePawnAttributeSet : public URogueHealthAttributeSet
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category="MoveSpeed")
	FRogueAttribute MoveSpeed;
	
	UPROPERTY(EditAnywhere, Category="MoveSpeed")
	FRogueAttribute MoveSpeedMultiplier;
	
	void ApplyMoveSpeed();
	
public:
	
	virtual void InitializeAttribute() override;
	
	virtual void PostAttributeChanged() override;
	
	URoguePawnAttributeSet();
};

UCLASS()
class URoguePlayerAttributeSet : public URoguePawnAttributeSet
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category="Resources")
	FRogueAttribute Rage;
	
	UPROPERTY(EditAnywhere, Category="Resources")
	FRogueAttribute RageMax;
	
public:
	
	virtual void PostAttributeChanged() override;
	
	URoguePlayerAttributeSet();
};

UCLASS()
class URogueMonsterAttributeSet : public URoguePawnAttributeSet
{
	GENERATED_BODY()

public:
	
	URogueMonsterAttributeSet();
};
