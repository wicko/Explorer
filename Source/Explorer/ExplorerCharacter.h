// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DamageInterface.h"
#include "PaperCharacter.h"
#include "PickupActor.h"
#include "ExplorerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AExplorerCharacter : public APaperCharacter, public IDamageInterface
{
	GENERATED_BODY()


	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	
public:
	AExplorerCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class UTP_WeaponComponent* equippedWeapon = nullptr;

	/** Scene Component for weapon sprite anchor*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	USceneComponent* weaponAnchor = nullptr;

	/** the id of the equipped weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	uint8 equippedWeaponID;

	/** Current Weapon Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TArray<FWeapon> weaponInventory;

	/** Called when removing the player weapon and replacing it with another */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SwapWeapons(FWeapon newWeapon);

	//Check whether the player has that current weapon in their inventory already
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool HasWeapon(FWeapon newWeapon);

	//Add ammo to your current weapon. Return false if ammo is maxed
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool CollectAmmo(FWeaponAmmo ammo, int32 ammoNum);

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle, class UTP_WeaponComponent* weapon);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	//logic for interacting with pickup actors. true if item is picked up and can be destroyed
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool InteractWithPickup(APickupActor* pickup);

	/** Getter for the currently equipped weapon */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	class UTP_WeaponComponent* GetEquippedWeapon() { return equippedWeapon; };


	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface






};

