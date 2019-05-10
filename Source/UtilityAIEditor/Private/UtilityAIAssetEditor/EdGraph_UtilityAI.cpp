#include "EdGraph_UtilityAI.h"
#include "UtilityAIEditorPCH.h"
#include "UtilityAIGraph.h"
#include "EdNode_UtilityAINode.h"
#include "EdNode_UtilityAIEdge.h"

UEdGraph_UtilityAI::UEdGraph_UtilityAI()
{

}

UEdGraph_UtilityAI::~UEdGraph_UtilityAI()
{

}

void UEdGraph_UtilityAI::RebuildUtilityAI()
{
	LOG_INFO(TEXT("UUtilityAIEdGraph::RebuildUtilityAI has been called"));

	UUtilityAIGraph* Graph = GetUtilityAI();

	Clear();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_UtilityAINode* EdNode = Cast<UEdNode_UtilityAINode>(Nodes[i]))
		{
			if (EdNode->UtilityAINode == nullptr)
				continue;

			UUtilityAINode* UtilityAINode = EdNode->UtilityAINode;

			NodeMap.Add(UtilityAINode, EdNode);

			Graph->AllNodes.Add(UtilityAINode);

			for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = EdNode->Pins[PinIdx];

				if (Pin->Direction != EEdGraphPinDirection::EGPD_Output)
					continue;

				for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
				{
					UUtilityAINode* ChildNode = nullptr;
					if (UEdNode_UtilityAINode* EdNode_Child = Cast<UEdNode_UtilityAINode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						ChildNode = EdNode_Child->UtilityAINode;
					}
					else if (UEdNode_UtilityAIEdge* EdNode_Edge = Cast<UEdNode_UtilityAIEdge>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						UEdNode_UtilityAINode* Child = EdNode_Edge->GetEndNode();;
						if (Child != nullptr)
						{
							ChildNode = Child->UtilityAINode;
						}
					}

					if (ChildNode != nullptr)
					{
						UtilityAINode->ChildrenNodes.Add(ChildNode);

						ChildNode->ParentNodes.Add(UtilityAINode);
					}
					else
					{
						LOG_ERROR(TEXT("UEdGraph_UtilityAI::RebuildUtilityAI can't find child node"));
					}
				}
			}
		}
		else if (UEdNode_UtilityAIEdge* EdgeNode = Cast<UEdNode_UtilityAIEdge>(Nodes[i]))
		{
			UEdNode_UtilityAINode* StartNode = EdgeNode->GetStartNode();
			UEdNode_UtilityAINode* EndNode = EdgeNode->GetEndNode();
			UUtilityAIEdge* Edge = EdgeNode->UtilityAIEdge;

			if (StartNode == nullptr || EndNode == nullptr || Edge == nullptr)
			{
				LOG_ERROR(TEXT("UEdGraph_UtilityAI::RebuildUtilityAI add edge failed."));
				continue;
			}

			EdgeMap.Add(Edge, EdgeNode);

			Edge->Graph = Graph;
			Edge->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
			Edge->StartNode = StartNode->UtilityAINode;
			Edge->EndNode = EndNode->UtilityAINode;
			Edge->StartNode->Edges.Add(Edge->EndNode, Edge);
		}
	}

	for (int i = 0; i < Graph->AllNodes.Num(); ++i)
	{
		UUtilityAINode* Node = Graph->AllNodes[i];
		if (Node->ParentNodes.Num() == 0)
		{
			Graph->RootNodes.Add(Node);

			SortNodes(Node);
		}

		Node->Graph = Graph;
		Node->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
	}

	Graph->RootNodes.Sort([&](const UUtilityAINode& L, const UUtilityAINode& R)
	{
		UEdNode_UtilityAINode* EdNode_LNode = NodeMap[&L];
		UEdNode_UtilityAINode* EdNode_RNode = NodeMap[&R];
		return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
	});
}

UUtilityAIGraph* UEdGraph_UtilityAI::GetUtilityAI() const
{
	return CastChecked<UUtilityAIGraph>(GetOuter());
}

bool UEdGraph_UtilityAI::Modify(bool bAlwaysMarkDirty /*= true*/)
{
	bool Rtn = Super::Modify(bAlwaysMarkDirty);

	GetUtilityAI()->Modify();

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}

	return Rtn;
}

void UEdGraph_UtilityAI::Clear()
{
	UUtilityAIGraph* Graph = GetUtilityAI();

	Graph->ClearGraph();
	NodeMap.Reset();
	EdgeMap.Reset();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_UtilityAINode* EdNode = Cast<UEdNode_UtilityAINode>(Nodes[i]))
		{
			UUtilityAINode* UtilityAINode = EdNode->UtilityAINode;
			UtilityAINode->ParentNodes.Reset();
			UtilityAINode->ChildrenNodes.Reset();
			UtilityAINode->Edges.Reset();
		}
	}
}

void UEdGraph_UtilityAI::SortNodes(UUtilityAINode* RootNode)
{
	int Level = 0;
	TArray<UUtilityAINode*> CurrLevelNodes = { RootNode };
	TArray<UUtilityAINode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		int32 LevelWidth = 0;
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UUtilityAINode* Node = CurrLevelNodes[i];

			auto Comp = [&](const UUtilityAINode& L, const UUtilityAINode& R)
			{
				UEdNode_UtilityAINode* EdNode_LNode = NodeMap[&L];
				UEdNode_UtilityAINode* EdNode_RNode = NodeMap[&R];
				return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
			};

			Node->ChildrenNodes.Sort(Comp);
			Node->ParentNodes.Sort(Comp);

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

void UEdGraph_UtilityAI::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}

