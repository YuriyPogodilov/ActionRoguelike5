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
	
	BurningEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BurningEffect"));
	BurningEffectComp->SetupAttachment(MeshComp);
	BurningEffectComp->SetAutoActivate(false);
	
	LoopBurningSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopBurningSound"));
	LoopBurningSoundComp->SetupAttachment(MeshComp);
	LoopBurningSoundComp->SetAutoActivate(false);
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(MeshComp);
	RadialForceComponent->Radius = 300.f;
	RadialForceComponent->ImpulseStrength = 500000.f;
	
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
	BurningEffectComp->Activate();
	LoopBurningSoundComp->Activate();
	
	FTimerHandle ExplosionTimerHandle;
	
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ARogueExplosive::Explode, ExplosionDelay);
}

void ARogueExplosive::Explode()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	
	float ExplosionRadius = RadialForceComponent->Radius;
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), 
		ExplosionRadius, DamageTypeClass, IgnoreActors, this);
	
	RadialForceComponent->FireImpulse();
	
	Destroy();
}

