// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplorerProjectile.h"
#include "ExplorerProjectile_Beam.generated.h"


/**
 * 
 */
UCLASS()
class EXPLORER_API AExplorerProjectile_Beam : public AExplorerProjectile
{
	GENERATED_BODY()

	AExplorerProjectile_Beam();
	public:

	UPROPERTY(EditAnywhere, Category = Projectile)
	UNiagaraSystem* BeamEffect;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called when the game starts */
	virtual void BeginPlay() override;
};
