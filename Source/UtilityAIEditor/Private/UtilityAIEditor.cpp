#include "AssetTypeActions_UtilityAI.h"
#include "UtilityAIEditorPCH.h"
#include "EdGraphUtilities.h"
#include "UtilityAIAssetEditor/EdNode_UtilityAINode.h"
#include "UtilityAIAssetEditor/EdNode_UtilityAIEdge.h"
#include "UtilityAIAssetEditor/SEdNode_UtilityAINode.h"
#include "UtilityAIAssetEditor/SEdNode_UtilityAIEdge.h"
#include "UtilityAIAssetEditor/UtilityAIEditorStyle.h"

DEFINE_LOG_CATEGORY(UtilityAIEditor)

#define LOCTEXT_NAMESPACE "Editor_UtilityAI"

class FGraphPanelNodeFactory_UtilityAI : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UEdNode_UtilityAINode* EdNode_GraphNode = Cast<UEdNode_UtilityAINode>(Node))
		{
			return SNew(SEdNode_UtilityAINode, EdNode_GraphNode);
		}
		else if (UEdNode_UtilityAIEdge* EdNode_Edge = Cast<UEdNode_UtilityAIEdge>(Node))
		{
			return SNew(SEdNode_UtilityAIEdge, EdNode_Edge);
		}
		return nullptr;
	}
};

TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_UtilityAI;

class FUtilityAIEditor : public IUtilityAIEditor
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

private:
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	EAssetTypeCategories::Type UtilityAIAssetCategoryBit;
};

IMPLEMENT_MODULE( FUtilityAIEditor, UtilityAIEditor )

void FUtilityAIEditor::StartupModule()
{
	FUtilityAIEditorStyle::Initialize();

	GraphPanelNodeFactory_UtilityAI = MakeShareable(new FGraphPanelNodeFactory_UtilityAI());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_UtilityAI);

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	UtilityAIAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("UtilityAI")), LOCTEXT("UtilityAIAssetCategory", "UtilityAI"));

	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_UtilityAI(UtilityAIAssetCategoryBit)));
}


void FUtilityAIEditor::ShutdownModule()
{
	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}

	if (GraphPanelNodeFactory_UtilityAI.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_UtilityAI);
		GraphPanelNodeFactory_UtilityAI.Reset();
	}

	FUtilityAIEditorStyle::Shutdown();
}

void FUtilityAIEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE

