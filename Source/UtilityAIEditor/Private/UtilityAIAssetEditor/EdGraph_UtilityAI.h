#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_UtilityAI.generated.h"

class UUtilityAIGraph;
class UUtilityAINode;
class UUtilityAIEdge;
class UEdNode_UtilityAINode;
class UEdNode_UtilityAIEdge;

UCLASS()
class UEdGraph_UtilityAI : public UEdGraph
{
	GENERATED_BODY()

public:
	UEdGraph_UtilityAI();
	virtual ~UEdGraph_UtilityAI();

	virtual void RebuildUtilityAI();

	UUtilityAIGraph* GetUtilityAI() const;

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void PostEditUndo() override;

	UPROPERTY(Transient)
	TMap<UUtilityAINode*, UEdNode_UtilityAINode*> NodeMap;

	UPROPERTY(Transient)
	TMap<UUtilityAIEdge*, UEdNode_UtilityAIEdge*> EdgeMap;

protected:
	void Clear();

	void SortNodes(UUtilityAINode* RootNode);
};
