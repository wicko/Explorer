// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplorerProjectile.h"
#include "ExplorerProjectile_Bullet.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class EXPLORER_API AExplorerProjectile_Bullet : public AExplorerProjectile
{
	GENERATED_BODY()

public:
	AExplorerProjectile_Bullet();

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** called when projectile hits something */

};
