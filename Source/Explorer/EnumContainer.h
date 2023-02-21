// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EGameModes : uint8
{
	GM_BOOTUP		 UMETA(DisplayName = "Bootup"),
	GM_LOADSCREEN	 UMETA(DisplayName = "Loadscreen"),
	GM_MAINMENU		 UMETA(DisplayName = "MainMenu"),
	GM_MISSION		 UMETA(DisplayName = "TownHub"),

};
