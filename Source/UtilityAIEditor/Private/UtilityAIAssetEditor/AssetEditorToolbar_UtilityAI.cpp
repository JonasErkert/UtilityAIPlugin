#include "AssetEditorToolbar_UtilityAI.h"
#include "AssetEditor_UtilityAI.h"
#include "EditorCommands_UtilityAI.h"
#include "UtilityAIEditorStyle.h"

#define LOCTEXT_NAMESPACE "AssetEditorToolbar_UtilityAI"

void FAssetEditorToolbar_UtilityAI::AddUtilityAIToolbar(TSharedPtr<FExtender> Extender)
{
	check(UtilityAIEditor.IsValid());
	TSharedPtr<FAssetEditor_UtilityAI> UtilityAIEditorPtr = UtilityAIEditor.Pin();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, UtilityAIEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP( this, &FAssetEditorToolbar_UtilityAI::FillUtilityAIToolbar ));
	UtilityAIEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FAssetEditorToolbar_UtilityAI::FillUtilityAIToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(UtilityAIEditor.IsValid());
	TSharedPtr<FAssetEditor_UtilityAI> UtilityAIEditorPtr = UtilityAIEditor.Pin();

	ToolbarBuilder.BeginSection("Utility AI");
	{
		ToolbarBuilder.AddToolBarButton(FEditorCommands_UtilityAI::Get().GraphSettings,
			NAME_None,
			LOCTEXT("GraphSettings_Label", "Graph Settings"),
			LOCTEXT("GraphSettings_ToolTip", "Show the Graph Settings"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings"));
	}
	ToolbarBuilder.EndSection();

	ToolbarBuilder.BeginSection("Util");
	{
		ToolbarBuilder.AddToolBarButton(FEditorCommands_UtilityAI::Get().AutoArrange,
			NAME_None,
			LOCTEXT("AutoArrange_Label", "Auto Arrange"),
			LOCTEXT("AutoArrange_ToolTip", "Auto arrange nodes, not perfect, but still handy"),
			FSlateIcon(FUtilityAIEditorStyle::GetStyleSetName(), "UtilityAIEditor.AutoArrange"));
	}
	ToolbarBuilder.EndSection();

}


#undef LOCTEXT_NAMESPACE
