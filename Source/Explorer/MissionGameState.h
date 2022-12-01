// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MissionGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMissionComplete);

/**
 * 
 */
UCLASS()
class EXPLORER_API AMissionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMissionGameState();

	UPROPERTY(BlueprintAssignable)
	FOnMissionComplete OnMissionComplete;

	UFUNCTION(BlueprintCallable)
	void SetMissionComplete(bool complete);

	UFUNCTION()
	bool IsMissionComplete() const;

private:
	bool bMissionComplete;
};
