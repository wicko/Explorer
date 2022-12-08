// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "DamageInterface.h"
#include "PaperCharacterLimb.generated.h"

UENUM(BlueprintType)
enum class ELimbName : uint8
{
		LIMB_TORSO			 UMETA(DisplayName = "Torso"),
		LIMB_HEAD			 UMETA(DisplayName = "Head"),
		LIMB_LEFT_ARM		 UMETA(DisplayName = "Left Arm"),
		LIMB_RIGHT_ARM		 UMETA(DisplayName = "Right Arm"),
		LIMB_LEFT_LEG		 UMETA(DisplayName = "Left Leg"),
		LIMB_RIGHT_LEG		 UMETA(DisplayName = "Right Leg"),

};
USTRUCT(BlueprintType)
struct FLimbData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELimbName limbName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 limbHealth = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isLimbRemoved = false;

};
/**
 * 
 */
UCLASS()
class EXPLORER_API UPaperCharacterLimb : public UPaperFlipbookComponent
{
	GENERATED_BODY()

public:

	UPaperCharacterLimb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLimbData limbData;

};
