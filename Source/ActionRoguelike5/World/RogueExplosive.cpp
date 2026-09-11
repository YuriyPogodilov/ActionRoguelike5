#include "RogueExplosive.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


ARogueExplosive::ARogueExplosive()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName("PhysicsActor");
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(MeshComp);
	RadialForceComponent->Radius = 500.f;
	RadialForceComponent->ImpulseStrength = 150000.f;
	
	ExplosionDelay = 3.f;
	ExplosionDamage = 30.f;
}

float ARogueExplosive::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	StartBurning();
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ARogueExplosive::StartBurning()
{
	if (bExploded || GetWorldTimerManager().TimerExists(ExplosionTimerHandle))
	{
		return;
	}
	
	ActiveBurningEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(BurningEffect, MeshComp, NAME_None,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	
	ActiveBurningSound = UGameplayStatics::SpawnSoundAttached(BurningSound, MeshComp);
	
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ARogueExplosive::Explode, ExplosionDelay);
}

void ARogueExplosive::Explode()
{
	bExploded = true;
	
	ActiveBurningEffect->Deactivate();
	ActiveBurningSound->Stop();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	
	// Apply explosion damage
	float ExplosionRadius = RadialForceComponent->Radius;
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), 
		ExplosionRadius, DamageTypeClass, IgnoreActors, this);
	
	RadialForceComponent->FireImpulse();
	
	// TODO: instead of destroying the barrel should fly up and change mesh to exploded
	Destroy();
}

