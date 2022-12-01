// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "ExplorerPaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORER_API AExplorerPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

	void UpdateAnimation();

};
