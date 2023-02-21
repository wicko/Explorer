// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplorerCharacter.h"
#include "ExplorerProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "WeaponDataAsset.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AExplorerCharacter

AExplorerCharacter::AExplorerCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	GetSprite()->SetupAttachment(FirstPersonCameraComponent);
	weaponAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAnchor"));
	weaponAnchor->SetupAttachment(FirstPersonCameraComponent);



}

void AExplorerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AExplorerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExplorerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExplorerCharacter::Look);
	}
}


void AExplorerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AExplorerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AExplorerCharacter::SwapWeapons(FWeapon newWeapon)
{
	if (equippedWeapon)
	{
		equippedWeapon->DestroyComponent();
	}
	equippedWeapon = NewObject<UTP_WeaponComponent>(this, newWeapon.weapon);
	equippedWeapon->RegisterComponent();
	equippedWeapon->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	equippedWeapon->AttachWeapon(this);
}

void AExplorerCharacter::SetHasRifle(bool bNewHasRifle, UTP_WeaponComponent* weapon)
{
	bHasRifle = bNewHasRifle;
	equippedWeapon = weapon;
}

bool AExplorerCharacter::GetHasRifle()
{
	return bHasRifle;
}
bool AExplorerCharacter::HasWeapon(FWeapon newWeapon)
{
	for (FWeapon weapon : weaponInventory)
	{
		if (newWeapon.name == weapon.name)
		{
			return true;
		}
	}
	return false;
}

bool AExplorerCharacter::CollectAmmo(FWeaponAmmo ammo, int32 ammoNum)
{
	for (FWeapon weapon : weaponInventory)
	{
		if (ammo.name == weapon.ammo.name)
		{
			if (weapon.currentAmmo >= weapon.maxAmmo)
			{
				return false;
			}
			else
			{
				weapon.currentAmmo = FMath::Clamp(weapon.currentAmmo + ammoNum, 0, weapon.maxAmmo);
				return true;
			}
		
		}
	}
	return false;
}

bool AExplorerCharacter::InteractWithPickup(APickupActor* pickup)
{
	if (pickup->pickUpData.PickUpTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Pickup.Weapon"))))
	{
		if (!HasWeapon(pickup->weaponType->weaponData))
		{
			FWeapon newWeapon = FWeapon();
			newWeapon.weapon = pickup->weaponType->weaponData.weapon;
			newWeapon.currentAmmo = pickup->pickUpData.amount;
			weaponInventory.Add(newWeapon);
			SwapWeapons(newWeapon);
		}
		else
		{
			return CollectAmmo(pickup->weaponType->weaponData.ammo, pickup->pickUpData.amount);
		}
	}
	else if (pickup->pickUpData.PickUpTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Pickup.Ammo"))))
	{
		return CollectAmmo(pickup->weaponType->weaponData.ammo, pickup->pickUpData.amount);
	}
	return true;
}
