// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoguePlayerController.generated.h"

class UInputAction;
class URogueInteractionComponent;


UCLASS()
class ACTIONROGUELIKE5_API ARoguePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<URogueInteractionComponent> InteractionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Interact;
	
	virtual void SetupInputComponent() override;
	
public:
	ARoguePlayerController();
	
	void StartInteract();
};
