// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageInterface.h"
#include "DisplayTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORER_API UDisplayTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyDamagePopup(FDamagePackage damagePack);

};
