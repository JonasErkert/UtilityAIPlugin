#include "EditorCommands_UtilityAI.h"

#define LOCTEXT_NAMESPACE "EditorCommands_UtilityAI"

void FEditorCommands_UtilityAI::RegisterCommands()
{
	UI_COMMAND(GraphSettings, "Graph Settings", "Graph Settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
