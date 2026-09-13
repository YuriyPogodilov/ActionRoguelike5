// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBlackHoleProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ARogueBlackHoleProjectile::ARogueBlackHoleProjectile()
{
	SphereComponent->SetCollisionProfileName("OverlapAll");
	
	ProjectileMovementComponent->InitialSpeed = 800.f;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	RadialForceComponent->ForceStrength = -2000000.f;
	RadialForceComponent->Radius = 500.f;
}

void ARogueBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARogueBlackHoleProjectile::TimerElapsed, Duration);
}

void ARogueBlackHoleProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueBlackHoleProjectile::OnActorOverlapBegin);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ARogueBlackHoleProjectile::TimerElapsed()
{
	Destroy();
}

void ARogueBlackHoleProjectile::OnActorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}
