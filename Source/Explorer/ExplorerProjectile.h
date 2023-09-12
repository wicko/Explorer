// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageInterface.h"
#include "PaperFlipbookComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"
#include "ExplorerProjectile.generated.h"




UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	HITSCAN		 UMETA(DisplayName = "Hitscan"),
	PROJECTILE	 UMETA(DisplayName = "Projectile"),
	SPECIAL		 UMETA(DisplayName = "SpecialCase"),

};

UCLASS(config=Game)
class AExplorerProjectile : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TArray<TEnumAsByte<EObjectTypeQuery>> HitObjectTypes;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UPaperFlipbookComponent* projectileSprite = nullptr;

	/** the type of projectile this is for creation  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EProjectileType projType = EProjectileType::HITSCAN;

	/** the effect created at impact **/
	UPROPERTY(EditAnywhere, Category = Projectile)
	UNiagaraSystem* contactEffect;

	/** whether or not the projectile continues after colliding */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canBounce = false;

	/** if the projectile is an instant hitscan, find the endpoint*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> endPoints;

	/**if the projectile can pierce, how deep they can pierce  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pierceDepth = 0.0f;

	/**if not a lifetime duration, the distance that the projectile can move (for things like beams)  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float distance = 0.0f;

	//where the origin of the projectile spawn is
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform spawnTrans = FTransform();

	/** the damage package associated with this projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamagePackage dmgPack = FDamagePackage();

	AExplorerProjectile();




protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** called when projectile hits something */
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	APlayerCameraManager* playerCamera = nullptr;
private:

};

