#pragma once

#include "CoreMinimal.h"

class FEditorCommands_UtilityAI : public TCommands<FEditorCommands_UtilityAI>
{
public:
	/** Constructor */
	FEditorCommands_UtilityAI()
		: TCommands<FEditorCommands_UtilityAI>("UtilityAIEditor", NSLOCTEXT("Contexts", "UtilityAIEditor", "Utility AI Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}
	
	TSharedPtr<FUICommandInfo> GraphSettings;
	TSharedPtr<FUICommandInfo> AutoArrange;

	virtual void RegisterCommands() override;
};
