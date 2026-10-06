#include "RogueProjectileMagic.h"

#include "ActionSystem/RogueActionEffect.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueProjectileMagic::ARogueProjectileMagic()
{
	ProjectileMovementComponent->InitialSpeed = 2000.0f;
	
	InitialLifeSpan = 8.0f;
}

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector HitFromDirection = GetActorRotation().Vector();
	UGameplayStatics::ApplyPointDamage(OtherActor, AttackDamage, HitFromDirection, Hit, GetInstigatorController(), this, nullptr);
	
	if (OtherComp->IsSimulatingPhysics(Hit.BoneName))
	{
		OtherComp->AddImpulseAtLocation(HitFromDirection * ImpulseIntensity, Hit.Location, Hit.BoneName);
	}
	
	if (IsValid(ApplyingEffect))
	{
		if (URogueActionSystemComponent* OtherActionSystemComp = OtherActor->GetComponentByClass<URogueActionSystemComponent>())
		{
			OtherActionSystemComp->GrantAction(ApplyingEffect);
		}
	}
	
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
