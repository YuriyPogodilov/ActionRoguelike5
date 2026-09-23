// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction_ProjectileAttack.h"

#include "NiagaraFunctionLibrary.h"
#include "RogueActionSystemComponent.h"
#include "RogueGameTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectile.h"


TAutoConsoleVariable<float> CVarProjectileAdjustmentDebugDrawing(TEXT("game.projectile.DebugDraw"), 0.f, 
	TEXT("Enable projectile aim adjustment debug rendering. (0 = off, > 0 is duration)"),
	ECVF_Cheat);

URogueAction_ProjectileAttack::URogueAction_ProjectileAttack()
{
	MuzzleSocketName = "Muzzle_01";
	
	CooldownTime = 0.5f;
}

void URogueAction_ProjectileAttack::StartAction_Implementation()
{
	Super::StartAction_Implementation();
	
	URogueActionSystemComponent* ActionComp = GetOwningComponent();
	ACharacter* Character = CastChecked<ACharacter>(ActionComp->GetOwner());
	
	Character->PlayAnimMontage(AttackMontage);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, Character->GetMesh(), MuzzleSocketName, 
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	UGameplayStatics::PlaySound2D(this, CastingSound);
	
	FTimerHandle AbilityTimerHandle;
	FTimerDelegate AbilityTimerDelegate;
	AbilityTimerDelegate.BindUObject(this, &ThisClass::AttackTimerElapsed); 
	
	GetWorld()->GetTimerManager().SetTimer(AbilityTimerHandle, AbilityTimerDelegate, AnimationDelay, false);
}

void URogueAction_ProjectileAttack::AttackTimerElapsed()
{
	URogueActionSystemComponent* ActionComp = GetOwningComponent();
	ACharacter* Character = CastChecked<ACharacter>(ActionComp->GetOwner());
	
	FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(MuzzleSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	FVector EyeLocation;
	FRotator EyeRotation;
	Character->GetController()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 5000.f);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	
	UWorld* World = GetWorld();
	
	FVector AdjustedTargetLocation;
	
	FHitResult HitResult;
	if (World->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams))
	{
		AdjustedTargetLocation = HitResult.Location;
	}
	else
	{
		AdjustedTargetLocation = TraceEnd;
	}
	
	FRotator SpawnRotation = (AdjustedTargetLocation - SpawnLocation).Rotation();
	
	AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	Character->MoveIgnoreActorAdd(NewProjectile);
	
	StopAction_Implementation();
	
#if !UE_BUILD_SHIPPING
	float DrawDebugDuration = CVarProjectileAdjustmentDebugDrawing.GetValueOnGameThread();
	
	if (DrawDebugDuration > 0.f)
	{
		// the hit location or trace end
		DrawDebugBox(World, AdjustedTargetLocation, FVector(20.f), FColor::Green, false, DrawDebugDuration);
	
		// adjustment line trace
		DrawDebugLine(World, EyeLocation, AdjustedTargetLocation, FColor::Green, false, DrawDebugDuration);

		// New projectile path
		DrawDebugLine(World, SpawnLocation, AdjustedTargetLocation, FColor::Yellow, false, DrawDebugDuration);
	
		// the original path of the projectile
		DrawDebugLine(World, SpawnLocation, (SpawnLocation + EyeRotation.Vector() * 5000.f), FColor::Purple, false, DrawDebugDuration);
	}
#endif
}
