// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickable.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


ARoguePickable::ARoguePickable()
{
	OverlapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = OverlapComponent;
	OverlapComponent->SetSphereRadius(64.0f);
	OverlapComponent->SetCollisionProfileName("Pickups");
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetCollisionProfileName("NoCollision");
	MeshComponent->SetupAttachment(OverlapComponent);
}

void ARoguePickable::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ARoguePickable::OnActorOverlapBegin);
}

void ARoguePickable::OnActorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUp(OtherActor);
}

void ARoguePickable::PickUp(AActor* OtherActor)
{
	if (!ApplyPickupEffect(OtherActor))
	{
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
	
	Destroy();
}

bool ARoguePickable::ApplyPickupEffect_Implementation(AActor* OtherActor)
{
	return false;
}

