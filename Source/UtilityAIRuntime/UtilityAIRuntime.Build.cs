using UnrealBuildTool;

public class UtilityAIRuntime : ModuleRules
{
	public UtilityAIRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange( new string[] { } );
		PrivateIncludePaths.AddRange( new string[] { "UtilityAIRuntime/Private" } );

		PublicDependencyModuleNames.AddRange( new string[] { "Core", "CoreUObject", "Engine" } );
		PrivateDependencyModuleNames.AddRange( new string[] { "Slate", "SlateCore", "GameplayTags" } );

		if (Target.bBuildDeveloperTools || (Target.Configuration != UnrealTargetConfiguration.Shipping && Target.Configuration != UnrealTargetConfiguration.Test))
		{
			PrivateDependencyModuleNames.Add("GameplayDebugger");
			PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=1");
		}
		else
		{
			PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=0");
		}

		DynamicallyLoadedModuleNames.AddRange( new string[] { } );
	}
}