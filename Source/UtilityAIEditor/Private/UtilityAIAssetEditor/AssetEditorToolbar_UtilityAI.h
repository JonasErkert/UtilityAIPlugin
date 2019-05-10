
#pragma once

#include "CoreMinimal.h"

class FAssetEditor_UtilityAI;
class FExtender;
class FToolBarBuilder;

class FAssetEditorToolbar_UtilityAI : public TSharedFromThis<FAssetEditorToolbar_UtilityAI>
{
public:
	FAssetEditorToolbar_UtilityAI(TSharedPtr<FAssetEditor_UtilityAI> InUtilityAIEditor)
		: UtilityAIEditor(InUtilityAIEditor) {}

	void AddUtilityAIToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillUtilityAIToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	/** Pointer back to the blueprint editor tool that owns us */
	TWeakPtr<FAssetEditor_UtilityAI> UtilityAIEditor;
};
