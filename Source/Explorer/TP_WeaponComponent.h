// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "DamageInterface.h"
#include "TP_WeaponComponent.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NONE		 UMETA(DisplayName = "None"),
	PISTOL		 UMETA(DisplayName = "Pistol"),
	RIFLE		 UMETA(DisplayName = "Rifle"),

};


class AExplorerCharacter;


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXPLORER_API UTP_WeaponComponent : public UPaperFlipbookComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType name = EWeaponType::NONE;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AExplorerProjectile> ProjectileClass;

	/** If the weapon has any secondary projectiles, such as for alt fire, slot in here */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AExplorerProjectile> AltProjectileClass;

	// Animation for the weapon's idle state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UPaperFlipbook* idleAnim = nullptr;

	// Animation for weapon reload
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UPaperFlipbook* reloadAnim = nullptr;

	// Animation for weapon firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UPaperFlipbook* firingAnim = nullptr;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** if the weapon has a charge functionality, how long it takes to charge */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float chargeTime = 0.0f;

	/** how far the primary fire projectile moves (0 is infinite) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float primaryFireDistance = 0.0f;

	/** how far the alternate fire projectile moves (0 is infinite) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float altFireDistance = 1000.0f;

	/** AnimMontage to play each time we fire */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset = FVector(5.0f, 0.0f, 0.0f);

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HoldFireAction;


	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AExplorerCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	/** If the weapon has a charge functionality, start it here*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HoldFireStart(const FInputActionValue& Value);

	/** If the weapon has a charge functionality, on release, fire this*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HoldFireRelease(const FInputActionValue& Value);

	/** If the weapon has a charge functionality, if cancelled before full charge time is finished, fire this*/
	void CancelHold(const FInputActionValue& Value);

	/** the damage package set for this primary projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamagePackage dmgPack = FDamagePackage();

	/** the damage package set for the alternate projectile, if there is one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamagePackage altDmgPack = FDamagePackage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* debugMuzzleOrigin = nullptr;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void SetIdleAnim();

	virtual void BeginPlay();

private:
	/** The Character holding this weapon*/
	AExplorerCharacter* Character;

	void CreateProjectile(TSubclassOf<AExplorerProjectile> projectileType, bool isAltFire);

	float currentChargeTime = 0.0f;
	//the location of the projectile spawn socket location on the sprite
	FTransform socketLoc;


};
