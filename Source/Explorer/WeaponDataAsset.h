// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "TP_WeaponComponent.h"
#include "WeaponDataAsset.generated.h"

class AExplorerProjectile;


UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	NONE		 UMETA(DisplayName = "None"),
	PISTOL		 UMETA(DisplayName = "Pistol"),


};

USTRUCT(BlueprintType)
struct FWeaponAmmo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAmmoType name = EAmmoType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 damage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AExplorerProjectile> projectile;
};

USTRUCT(BlueprintType)
struct FWeapon : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType name = EWeaponType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTP_WeaponComponent> weapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 maxAmmo = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 currentAmmo = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FWeaponAmmo ammo;
};
/**
 * 
 */
UCLASS()
class EXPLORER_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeapon weaponData;

};
