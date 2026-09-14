// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueTeleportProjectile.h"

#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARogueTeleportProjectile::ARogueTeleportProjectile()
{
	ProjectileMovementComponent->InitialSpeed = 6000.f;
}

void ARogueTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARogueTeleportProjectile::StartDelayedTeleport, DetonateDelay);
}

void ARogueTeleportProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Do not call parent function as it destroying the projectile
	// Super::PlayExplosionEffects();
	
	StartDelayedTeleport();
}

void ARogueTeleportProjectile::StartDelayedTeleport()
{
	PlayExplosionEffects();
	
	ProjectileMovementComponent->StopMovementImmediately();
	
	LoopedNiagaraComponent->Deactivate();
	LoopedAudioComponent->Stop();
	SetActorEnableCollision(false);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARogueTeleportProjectile::HandleTeleportation, TeleportationDelay);
}

void ARogueTeleportProjectile::HandleTeleportation()
{
	APawn* ActorToTeleport = GetInstigator();
	
	check(ActorToTeleport);
	
	ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
	
	Destroy();
}
