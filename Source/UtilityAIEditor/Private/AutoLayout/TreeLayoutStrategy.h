#pragma once

#include "CoreMinimal.h"
#include "AutoLayoutStrategy.h"
#include "TreeLayoutStrategy.generated.h"

UCLASS()
class UTreeLayoutStrategy : public UAutoLayoutStrategy
{
	GENERATED_BODY()
public:
	UTreeLayoutStrategy();
	virtual ~UTreeLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) override;

protected:
	void InitPass(UUtilityAINode* RootNode, const FVector2D& Anchor);
	bool ResolveConflictPass(UUtilityAINode* Node);

	bool ResolveConflict(UUtilityAINode* LRoot, UUtilityAINode* RRoot);

	void GetLeftContour(UUtilityAINode* RootNode, int32 Level, TArray<UEdNode_UtilityAINode*>& Contour);
	void GetRightContour(UUtilityAINode* RootNode, int32 Level, TArray<UEdNode_UtilityAINode*>& Contour);
	
	void ShiftSubTree(UUtilityAINode* RootNode, const FVector2D& Offset);

	void UpdateParentNodePosition(UUtilityAINode* RootNode);
};
