#include "RogueProjectileMagic.h"

#include "Kismet/GameplayStatics.h"

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector HitFromDirection = GetActorRotation().Vector();
	UGameplayStatics::ApplyPointDamage(OtherActor, 10.f, HitFromDirection, Hit, GetInstigatorController(), this, DmgTypeClass);
	
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

