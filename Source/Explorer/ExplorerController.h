// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplorerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ExplorerController.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORER_API AExplorerController : public APlayerController
{
	GENERATED_BODY()

		AExplorerController();
public: 


	UFUNCTION()
	void CreateLoadScreen();

	/** Widget used for the player hud*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG") 
	TSubclassOf<UUserWidget> HUDWidgetClass;

	/** Widget used for loading screens*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;
private:

	AExplorerCharacter* playerPawn = nullptr;

	UUserWidget* currentWidget = nullptr;
	UUserWidget* appliedWidget = nullptr;
	void SwapWidget(UUserWidget* widget);
};
