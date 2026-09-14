// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectile.h"
#include "Abilities/RogueAbilityDataAsset.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ARoguePlayerCharacter::ARoguePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComponent"));
}

void ARoguePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ARoguePlayerCharacter::OnHealthChanged);
}

void ARoguePlayerCharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
	if (FMath::IsNearlyZero(NewHealth))
	{
		DisableInput(nullptr);
		
		GetMovementComponent()->StopMovementImmediately();
		
		PlayAnimMontage(DeathMontage);
	}
}

float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ActionSystemComponent->ApplyHealthChange(-DamageAmount);
	
	return ActualDamage;
}

void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Look);
	EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Jump);
	
	EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::PrimaryAttack);
	EnhancedInput->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::SecondaryAttack);
	EnhancedInput->BindAction(Input_Dash, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Dash);
}

void ARoguePlayerCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	
	// Forward/Backward
	AddMovementInput(ControlRotation.Vector(), InputValue.X);
	
	// Sideways
	FVector RightDirection = ControlRotation.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& InValue)
{
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ARoguePlayerCharacter::CastAbility(TObjectPtr<URogueAbilityDataAsset> AbilityDataAsset)
{
	const FRogueAbilityData AbilityData = AbilityDataAsset->AbilityData;
	
	PlayAnimMontage(AbilityData.AttackMontage);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(AbilityData.CastingEffect, GetMesh(), AbilityData.MuzzleSocketName, 
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	UGameplayStatics::PlaySound2D(this, AbilityData.CastingSound);
	
	FTimerHandle AbilityTimerHandle;
	FTimerDelegate AbilityTimerDelegate;
	AbilityTimerDelegate.BindUObject(this, &ARoguePlayerCharacter::AbilityTimerElapsed, AbilityDataAsset);
	
	GetWorldTimerManager().SetTimer(AbilityTimerHandle, AbilityTimerDelegate, AbilityData.AnimationDelay, false);
}

void ARoguePlayerCharacter::AbilityTimerElapsed(TObjectPtr<URogueAbilityDataAsset> AbilityDataAsset)
{
	const FRogueAbilityData AbilityData = AbilityDataAsset->AbilityData;
	
	FVector SpawnLocation = GetMesh()->GetSocketLocation(AbilityData.MuzzleSocketName);
	FRotator SpawnRotation = GetControlRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(AbilityData.ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	MoveIgnoreActorAdd(NewProjectile);
}

void ARoguePlayerCharacter::PrimaryAttack()
{
	CastAbility(PrimaryAbilityDataAsset);
}

void ARoguePlayerCharacter::SecondaryAttack()
{
	CastAbility(SecondaryAbilityDataAsset);
}

void ARoguePlayerCharacter::Dash()
{
	CastAbility(DashAbilityDataAsset);
}
