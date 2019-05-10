#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "TextProperty.h"
#include "UtilityAINode.generated.h"

class UUtilityAIGraph;
class UUtilityAIEdge;

UCLASS(Blueprintable)
class UTILITYAIRUNTIME_API UUtilityAINode : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAINode();
	virtual ~UUtilityAINode();

	UPROPERTY(VisibleDefaultsOnly, Category = "UtilityAINode")
	UUtilityAIGraph* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAINode")
	TArray<UUtilityAINode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAINode")
	TArray<UUtilityAINode*> ChildrenNodes;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAINode")
	TMap<UUtilityAINode*, UUtilityAIEdge*> Edges;

	UFUNCTION(BlueprintCallable, Category = "UtilityAINode")
	virtual UUtilityAIEdge* GetEdge(UUtilityAINode* ChildNode);

	UFUNCTION(BlueprintCallable, Category = "UtilityAINode")
	bool IsLeafNode() const;

	UFUNCTION(BlueprintCallable, Category = "UtilityAINode")
	UUtilityAIGraph* GetGraph() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MissionNode")
	FText GetDescription() const;
	virtual FText GetDescription_Implementation() const;

	//////////////////////////////////////////////////////////////////////////
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "UtilityAINode_Editor")
	FText NodeTitle;

	UPROPERTY(VisibleDefaultsOnly, Category = "UtilityAINode_Editor")
	TSubclassOf<UUtilityAIGraph> CompatibleGraphType;

	UPROPERTY(EditDefaultsOnly, Category = "UtilityAINode_Editor")
	FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category = "UtilityAINode_Editor")
	FText ContextMenuName;
#endif

#if WITH_EDITOR
	virtual FLinearColor GetBackgroundColor() const;

	virtual FText GetNodeTitle() const;

	virtual void SetNodeTitle(const FText& NewTitle);

	virtual bool CanCreateConnection(UUtilityAINode* Other, FText& ErrorMessage);
#endif
};
