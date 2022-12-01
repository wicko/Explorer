// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionGameState.h"

AMissionGameState::AMissionGameState()
{

}

void AMissionGameState::SetMissionComplete(bool complete)
{
	bMissionComplete = complete;
	OnMissionComplete.Broadcast();
}

bool AMissionGameState::IsMissionComplete() const
{
	return(bMissionComplete);
}