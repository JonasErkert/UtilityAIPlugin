// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UtilityAIComponent.h"
#include "UtilityAIController.generated.h"

class UUtilityAIGraph;

/**
 * This AIController derived class provides functions to execute the Utility AI Component.
 */
UCLASS()
class UTILITYAIRUNTIME_API AUtilityAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AUtilityAIController();

	/**
	 * Creates a new UtilityAIComponent and starts the execution of the utility ai graph.
	 * 
	 * @param UtilityAI			The graph to execute and evaluate.
	 * @param SelectionMethod	The method on how to select the action.
	 * @param TopN				Top n actions to select randomly from. Only matters when "Random Top n" is the SelectionMethod.
	 * @return The created UtilityAIComponent, null if creation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAIController")
	UUtilityAIComponent* RunUtilityAI(UUtilityAIGraph* UtilityAI, EScoreSelectionMethod SelectionMethod = EScoreSelectionMethod::ScoreSelectionMethod_Highest, int32 TopN = 3);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UtilityAIController")
	void ProvideConsiderationScores(UUtilityAIComponent* CreatedComponent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UtilityAIController")
	void ProvideDecisionImplementation(UUtilityAIComponent* CreatedComponent);
};
