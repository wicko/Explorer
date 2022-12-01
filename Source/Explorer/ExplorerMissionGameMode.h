// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplorerGameMode.h"
#include "ExplorerMissionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORER_API AExplorerMissionGameMode : public AExplorerGameMode
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds);
};
