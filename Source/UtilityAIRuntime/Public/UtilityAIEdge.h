#pragma once

#include "CoreMinimal.h"
#include "UtilityAINode.h"
#include "UtilityAIEdge.generated.h"

class UUtilityAIGraph;

UCLASS(Blueprintable)
class UTILITYAIRUNTIME_API UUtilityAIEdge : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAIEdge();
	virtual ~UUtilityAIEdge();

	UPROPERTY(VisibleAnywhere, Category = "UtilityAINode")
	UUtilityAIGraph* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAIEdge")
	UUtilityAINode* StartNode;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAIEdge")
	UUtilityAINode* EndNode;

	UFUNCTION(BlueprintPure, Category = "UtilityAIEdge")
	UUtilityAIGraph* GetGraph() const;
};
