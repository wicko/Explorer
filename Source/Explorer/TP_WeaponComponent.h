// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "TP_WeaponComponent.generated.h"


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
	
	/** AnimMontage to play each time we fire */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AExplorerCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

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


};
