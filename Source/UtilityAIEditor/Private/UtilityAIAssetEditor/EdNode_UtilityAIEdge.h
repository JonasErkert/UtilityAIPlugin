#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_UtilityAIEdge.generated.h"

class UUtilityAINode;
class UUtilityAIEdge;
class UEdNode_UtilityAINode;

UCLASS(MinimalAPI)
class UEdNode_UtilityAIEdge : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UEdGraph* Graph;

	UPROPERTY(VisibleAnywhere, Instanced, Category = "UtilityAI")
	UUtilityAIEdge* UtilityAIEdge;

	void SetEdge(UUtilityAIEdge* Edge);

	virtual void AllocateDefaultPins() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PrepareForCopying() override;

	virtual UEdGraphPin* GetInputPin() const { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	void CreateConnections(UEdNode_UtilityAINode* Start, UEdNode_UtilityAINode* End);

	UEdNode_UtilityAINode* GetStartNode();
	UEdNode_UtilityAINode* GetEndNode();
};
