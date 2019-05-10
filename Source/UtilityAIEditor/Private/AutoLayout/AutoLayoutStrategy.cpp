#include "AutoLayout/AutoLayoutStrategy.h"
#include "Kismet/KismetMathLibrary.h"
#include "UtilityAIAssetEditor/EdNode_UtilityAINode.h"
#include "UtilityAIAssetEditor/SEdNode_UtilityAINode.h"

UAutoLayoutStrategy::UAutoLayoutStrategy()
{
	Settings = nullptr;
	MaxIteration = 50;
	OptimalDistance = 150;
}

UAutoLayoutStrategy::~UAutoLayoutStrategy()
{

}

FBox2D UAutoLayoutStrategy::GetNodeBound(UEdGraphNode* EdNode)
{
	int32 NodeWidth = GetNodeWidth(Cast<UEdNode_UtilityAINode>(EdNode));
	int32 NodeHeight = GetNodeHeight(Cast<UEdNode_UtilityAINode>(EdNode));
	FVector2D Min(EdNode->NodePosX, EdNode->NodePosY);
	FVector2D Max(EdNode->NodePosX + NodeWidth, EdNode->NodePosY + NodeHeight);
	return FBox2D(Min, Max);
}

FBox2D UAutoLayoutStrategy::GetActualBounds(UUtilityAINode* RootNode)
{
	int Level = 0;
	TArray<UUtilityAINode*> CurrLevelNodes = { RootNode };
	TArray<UUtilityAINode*> NextLevelNodes;

	FBox2D Rtn = GetNodeBound(EdGraph->NodeMap[RootNode]);

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UUtilityAINode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			Rtn += GetNodeBound(EdGraph->NodeMap[Node]);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
	return Rtn;
}

void UAutoLayoutStrategy::RandomLayoutOneTree(UUtilityAINode* RootNode, const FBox2D& Bound)
{
	int Level = 0;
	TArray<UUtilityAINode*> CurrLevelNodes = { RootNode };
	TArray<UUtilityAINode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UUtilityAINode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			UEdNode_UtilityAINode* EdNode_Node = EdGraph->NodeMap[Node];

			EdNode_Node->NodePosX = UKismetMathLibrary::RandomFloatInRange(Bound.Min.X, Bound.Max.X);
			EdNode_Node->NodePosY = UKismetMathLibrary::RandomFloatInRange(Bound.Min.Y, Bound.Max.Y);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
}

int32 UAutoLayoutStrategy::GetNodeWidth(UEdNode_UtilityAINode* EdNode)
{
	return EdNode->SEdNode->GetCachedGeometry().GetLocalSize().X;
}

int32 UAutoLayoutStrategy::GetNodeHeight(UEdNode_UtilityAINode* EdNode)
{
	return EdNode->SEdNode->GetCachedGeometry().GetLocalSize().Y;
}

