#pragma once

#include "CoreMinimal.h"
#include "UtilityAINode.h"
#include "UtilityAIEdge.h"
#include "GameplayTagContainer.h"
#include "UtilityAIGraph.generated.h"

UCLASS(Blueprintable)
class UTILITYAIRUNTIME_API UUtilityAIGraph : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAIGraph();
	virtual ~UUtilityAIGraph();

	UPROPERTY(EditDefaultsOnly, Category = "UtilityAI")
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = "UtilityAI")
	TSubclassOf<UUtilityAINode> NodeType;

	UPROPERTY(EditDefaultsOnly, Category = "UtilityAI")
	TSubclassOf<UUtilityAIEdge> EdgeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAI")
	FGameplayTagContainer GraphTags;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAI")
	TArray<UUtilityAINode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAI")
	TArray<UUtilityAINode*> AllNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAI")
	bool bEdgeEnabled;

	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void Print(bool ToConsole = true, bool ToScreen = true);

	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	int GetLevelNum() const;

	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void GetNodesByLevel(int Level, TArray<UUtilityAINode*>& Nodes);

	void ClearGraph();

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UEdGraph* EdGraph;

	UPROPERTY(EditDefaultsOnly, Category = "UtilityAI_Editor")
	bool bCanRenameNode;
#endif
};
