#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "UtilityAINode.h"
#include "EdNode_UtilityAINode.generated.h"

class UEdNode_UtilityAIEdge;
class UEdGraph_UtilityAI;
class SEdNode_UtilityAINode;

UCLASS(MinimalAPI)
class UEdNode_UtilityAINode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UEdNode_UtilityAINode();
	virtual ~UEdNode_UtilityAINode();

	UPROPERTY(VisibleAnywhere, Instanced, Category = "UtilityAI")
	UUtilityAINode* UtilityAINode;

	void SetUtilityAINode(UUtilityAINode* InNode);
	UEdGraph_UtilityAI* GetUtilityAIEdGraph();

	SEdNode_UtilityAINode* SEdNode;

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual FLinearColor GetBackgroundColor() const;
	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif

};
