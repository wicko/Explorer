// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDCombatUMG.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORER_API UPlayerHUDCombatUMG : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 healthValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 armorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ammoValue;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth(uint8 newHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateArmor(uint8 newArmor);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateAmmo(uint8 newAmmo);
};
