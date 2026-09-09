#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueExplosive.generated.h"

class UNiagaraSystem;
class URadialForceComponent;
class UNiagaraComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE5_API ARogueExplosive : public AActor
{
	GENERATED_BODY()

public:
	ARogueExplosive();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void StartBurning();
	
	void Explode();
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UNiagaraComponent> BurningEffectComp;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UAudioComponent> LoopBurningSoundComp;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	TSubclassOf<UDamageType> DamageTypeClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionDelay;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionDamage;
};
