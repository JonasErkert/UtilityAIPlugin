#include "EdNode_UtilityAINode.h"
#include "EdGraph_UtilityAI.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "EdNode_UtilityAI"

UEdNode_UtilityAINode::UEdNode_UtilityAINode()
{
	bCanRenameNode = true;
}

UEdNode_UtilityAINode::~UEdNode_UtilityAINode()
{

}

void UEdNode_UtilityAINode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

UEdGraph_UtilityAI* UEdNode_UtilityAINode::GetUtilityAIEdGraph()
{
	return Cast<UEdGraph_UtilityAI>(GetGraph());
}

FText UEdNode_UtilityAINode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (UtilityAINode == nullptr)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return UtilityAINode->GetNodeTitle();
	}
}

void UEdNode_UtilityAINode::PrepareForCopying()
{
	UtilityAINode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_UtilityAINode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != nullptr)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

void UEdNode_UtilityAINode::SetUtilityAINode(UUtilityAINode* InNode)
{
	UtilityAINode = InNode;
}

FLinearColor UEdNode_UtilityAINode::GetBackgroundColor() const
{
	return UtilityAINode == nullptr ? FLinearColor::Black : UtilityAINode->GetBackgroundColor();
}

UEdGraphPin* UEdNode_UtilityAINode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_UtilityAINode::GetOutputPin() const
{
	return Pins[1];
}

void UEdNode_UtilityAINode::PostEditUndo()
{
	UEdGraphNode::PostEditUndo();
}

#undef LOCTEXT_NAMESPACE
