#include "EdNode_UtilityAIEdge.h"
#include "UtilityAIEdge.h"
#include "EdNode_UtilityAINode.h"

#define LOCTEXT_NAMESPACE "EdNode_UtilityAIEdge"

void UEdNode_UtilityAIEdge::SetEdge(UUtilityAIEdge* Edge)
{
	UtilityAIEdge = Edge;
}

void UEdNode_UtilityAIEdge::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Edge"), FName(), TEXT("In"));
	Inputs->bHidden = true;
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Edge"), FName(), TEXT("Out"));
	Outputs->bHidden = true;
}

FText UEdNode_UtilityAIEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText();
}

void UEdNode_UtilityAIEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		// Commit suicide; transitions must always have an input and output connection
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

void UEdNode_UtilityAIEdge::PrepareForCopying()
{
	UtilityAIEdge->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_UtilityAIEdge::CreateConnections(UEdNode_UtilityAINode* Start, UEdNode_UtilityAINode* End)
{
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	Start->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(Start->GetOutputPin());

	// This to next
	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	End->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(End->GetInputPin());
}

UEdNode_UtilityAINode* UEdNode_UtilityAIEdge::GetStartNode()
{
	if (Pins[0]->LinkedTo.Num() > 0)
	{
		return Cast<UEdNode_UtilityAINode>(Pins[0]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

UEdNode_UtilityAINode* UEdNode_UtilityAIEdge::GetEndNode()
{
	if (Pins[1]->LinkedTo.Num() > 0)
	{
		return Cast<UEdNode_UtilityAINode>(Pins[1]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

