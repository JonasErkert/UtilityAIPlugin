#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "UtilityAIGraph.h"
#include "UtilityAIAssetEditor/EdGraph_UtilityAI.h"
#include "UtilityAIAssetEditor/EdNode_UtilityAINode.h"
#include "UtilityAIAssetEditor/EdNode_UtilityAIEdge.h"
#include "UtilityAIAssetEditor/Settings_UtilityAIEditor.h"
#include "AutoLayoutStrategy.generated.h"

UCLASS(abstract)
class UAutoLayoutStrategy : public UObject
{
	GENERATED_BODY()
public:
	UAutoLayoutStrategy();
	virtual ~UAutoLayoutStrategy();

	virtual void Layout(UEdGraph* G) {};

	class UUtilityAIEditorSettings* Settings;

protected:
	int32 GetNodeWidth(UEdNode_UtilityAINode* EdNode);

	int32 GetNodeHeight(UEdNode_UtilityAINode* EdNode);

	FBox2D GetNodeBound(UEdGraphNode* EdNode);

	FBox2D GetActualBounds(UUtilityAINode* RootNode);

	virtual void RandomLayoutOneTree(UUtilityAINode* RootNode, const FBox2D& Bound);

protected:
	UUtilityAIGraph* Graph;
	UEdGraph_UtilityAI* EdGraph;
	int32 MaxIteration;
	int32 OptimalDistance;
};
