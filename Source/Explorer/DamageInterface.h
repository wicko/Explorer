// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

//Different forms a source of damage can occur from
UENUM(BlueprintType)
enum class EDamageForm : uint8
{
	DEFAULT		UMETA(DisplayName = "Default Damage"),
	POINT		UMETA(DisplayName = "Point Damage"),
	RADIAL		UMETA(DisplayName = "Radial Damage"),
	DOT			UMETA(DisplayName = "DOT Damage"),

};


//List of special damage modifiers
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	DT_BLUNT			UMETA(DisplayName = "Blunt"),
	DT_PIERCING			UMETA(DisplayName = "Piercing"),
};

USTRUCT(BlueprintType)
struct FDamagePackage
{
	GENERATED_USTRUCT_BODY()

	/** the amount of damage being applied */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 damage = 0;

	/** the amount of modifier being applied */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float modifierValue = 0.0f;

	/** Damage Type tied to the damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDamageType damageTypes = EDamageType::DT_BLUNT;

	/** the form the damage came in (eg radial, point, dot, etc)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDamageForm damageForm = EDamageForm::DEFAULT;

	/** what actor caused this damage to occur */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* damageSource = nullptr;

	/** the origin of the damage location (used for things like ragdolling from a radial damage point of origin or from where the damage connects via swing) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector damageOrigin;

	/** the impulse of the damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float impulse = 0.0f;

	/** the stagger value of the damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 stagger = 0;

	/** Describes the trace/location that caused this damage */
	UPROPERTY()
		FVector hitVector;

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXPLORER_API IDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Reaction")
		void ReactToDamage(FDamagePackage damagePackage, USceneComponent* limb);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Effect")
		bool OnDamageEffect(FVector DamageLocation, FRotator hitRotation);
};
