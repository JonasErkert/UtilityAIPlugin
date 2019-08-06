#include "UtilityAINode.h"
#include "UtilityAIGraph.h"

#define LOCTEXT_NAMESPACE "UtilityAINode"

UUtilityAINode::UUtilityAINode()
{
	bIsNormalizedCustomCurve = true;
	NodeDescription		= TEXT("Description.");
	ActionScore			= 0.f;
	ConsiderationScore	= 0.f;
	BookendMin			= 0.f;
	BookendMax			= 1.f;
	bUseWeight			= false;
	Weight				= 1.f;
	bInvertScore		= false;
	bRandomShift		= false;
	RandomShiftRange	= 0.5f;
	RandomShift			= 0.f;
	bIsShiftSet			= false;

#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UUtilityAIGraph::StaticClass();

	BackgroundColor = FLinearColor::Black;
#endif
}

UUtilityAINode::~UUtilityAINode()
{

}

UUtilityAIEdge* UUtilityAINode::GetEdge(UUtilityAINode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

FText UUtilityAINode::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "Utility AI Node");
}

FName UUtilityAINode::GetNodeName() const
{
	return NodeName;
}

#if WITH_EDITOR

FLinearColor UUtilityAINode::GetBackgroundColor() const
{
	return BackgroundColor;
}

FText UUtilityAINode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? GetDescription() : NodeTitle;
}

void UUtilityAINode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

bool UUtilityAINode::CanCreateConnection(UUtilityAINode* Other, FText& ErrorMessage)
{
	return true;
}

#endif

bool UUtilityAINode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}

UUtilityAIGraph* UUtilityAINode::GetGraph() const
{
	return Graph;
}

#undef LOCTEXT_NAMESPACE
