#include "AssetTypeActions_UtilityAI.h"
#include "UtilityAIEditorPCH.h"
#include "UtilityAIAssetEditor/AssetEditor_UtilityAI.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_UtilityAI"

FAssetTypeActions_UtilityAI::FAssetTypeActions_UtilityAI(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_UtilityAI::GetName() const
{
	return LOCTEXT("FUtilityAIAssetTypeActionsName", "Utility AI");
}

FColor FAssetTypeActions_UtilityAI::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FAssetTypeActions_UtilityAI::GetSupportedClass() const
{
	return UUtilityAIGraph::StaticClass();
}

void FAssetTypeActions_UtilityAI::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UUtilityAIGraph* Graph = Cast<UUtilityAIGraph>(*ObjIt))
		{
			TSharedRef<FAssetEditor_UtilityAI> NewGraphEditor(new FAssetEditor_UtilityAI());
			NewGraphEditor->InitUtilityAIAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

uint32 FAssetTypeActions_UtilityAI::GetCategories()
{
	return EAssetTypeCategories::Animation | MyAssetCategory;
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE