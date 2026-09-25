// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "RoguePlayerCharacter.generated.h"

struct FGameplayTag;
class URogueActionSystemComponent;
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
	
	UPROPERTY(EditDefaultsOnly, Category="Death")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Move;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Look;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Sprint;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_SecondaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_SpecialAttack;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	
	void OnHealthChanged(FGameplayTag AttributeTag, float NewHealth, float OldHealth);
	
public:
	ARoguePlayerCharacter();

	virtual void PostInitializeComponents() override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;
	
	void Move(const FInputActionValue& InValue);
	void Look(const FInputActionInstance& InValue);
	
	void StartAction(FGameplayTag InActionName);
	
	void StopAction(FGameplayTag InActionName);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
