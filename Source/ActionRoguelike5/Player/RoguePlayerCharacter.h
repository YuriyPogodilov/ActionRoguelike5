// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "RoguePlayerCharacter.generated.h"

class URogueAbilityDataAsset;
class UNiagaraSystem;
class ARogueProjectile;
struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class USoundBase;

UCLASS()
class ACTIONROGUELIKE5_API ARoguePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TObjectPtr<URogueAbilityDataAsset> PrimaryAbilityDataAsset;
	
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TObjectPtr<URogueAbilityDataAsset> SecondaryAbilityDataAsset;
	
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TObjectPtr<URogueAbilityDataAsset> DashAbilityDataAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Move;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Look;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_SecondaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Dash;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	void CastAbility(TObjectPtr<URogueAbilityDataAsset> AbilityDataAsset);
	void AbilityTimerElapsed(TObjectPtr<URogueAbilityDataAsset> AbilityDataAsset);
	
public:
	ARoguePlayerCharacter();

	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& InValue);
	void Look(const FInputActionInstance& InValue);
	
	void PrimaryAttack();
	void SecondaryAttack();
	void Dash();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
