// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Warrior : ModuleRules
{
	public Warrior(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayTags",
			"GameplayAbilities",
			"GameplayTasks",
			"AnimGraphRuntime",
			"MotionWarping",
			"Niagara"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Warrior",
			"Warrior/Variant_Platforming",
			"Warrior/Variant_Platforming/Animation",
			"Warrior/Variant_Combat",
			"Warrior/Variant_Combat/AI",
			"Warrior/Variant_Combat/Animation",
			"Warrior/Variant_Combat/Gameplay",
			"Warrior/Variant_Combat/Interfaces",
			"Warrior/Variant_Combat/UI",
			"Warrior/Variant_SideScrolling",
			"Warrior/Variant_SideScrolling/AI",
			"Warrior/Variant_SideScrolling/Gameplay",
			"Warrior/Variant_SideScrolling/Interfaces",
			"Warrior/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
