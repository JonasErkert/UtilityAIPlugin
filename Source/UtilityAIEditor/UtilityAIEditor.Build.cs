using UnrealBuildTool;

public class UtilityAIEditor : ModuleRules
{
	public UtilityAIEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange( new string[] { } );
		PrivateIncludePaths.AddRange( new string[] { "UtilityAIEditor/Private" } );

		PublicDependencyModuleNames.AddRange( new string[] { "Core", "CoreUObject", "Engine", "UnrealEd" } );
		PrivateDependencyModuleNames.AddRange( new string[] { "UtilityAIRuntime", "AssetTools", "Slate", "SlateCore", "GraphEditor", "PropertyEditor", "EditorStyle", "Kismet", "KismetWidgets", "ApplicationCore" } );

		DynamicallyLoadedModuleNames.AddRange( new string[] { } );
	}
}