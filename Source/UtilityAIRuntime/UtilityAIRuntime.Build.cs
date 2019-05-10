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

		DynamicallyLoadedModuleNames.AddRange( new string[] { } );
	}
}