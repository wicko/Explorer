// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Explorer : ModuleRules
{
	public Explorer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule" });
	}
}
