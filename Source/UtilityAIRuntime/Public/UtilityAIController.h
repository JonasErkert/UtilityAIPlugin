// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UtilityAIController.generated.h"

class UUtilityAINode;

/**
 * This AIController derived class provides functions to evaluate an Utility AI Graph.
 */
UCLASS()
class UTILITYAIRUNTIME_API AUtilityAIController : public AAIController
{
	GENERATED_BODY()
	
	/**
	 * Sorts given action scores from highest to lowest score.
	 * 
	 * @param	ActionScores Scores to sort.
	 * @return	Sorted action scores, descending order.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	TArray<float> SortActionScores(const TArray<float>& ActionScores);

	/**
	 * Maps the ValueToNormalize with the defined bookends to 0 - 1, then gets the corresponding value of the custom response curve.
	 * Stores the calculated consideration score in the node itself and returns it.
	 * Also writes the calculated score into the ConsiderationScores array, which is used by SetActionScore()
	 * 
	 * @param CurrentConsiderationNode	Consideration node to retrieve bookends, the custom response curve and to save calculated score within.
	 * @param ValueToNormalize			The value to normalize and evaluate.
	 * @see SetActionScore()
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	float SetConsiderationScore(UUtilityAINode* CurrentConsiderationNode, float ValueToEvaluate);

	/**
	 * Iterates over the ConsiderationScores, multiplies each score and saves the calculated action score into the current action node.
	 * 
	 * @param CurrentActionNode The current action node the multiplied consideration scores get saved into.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SetActionScore(UUtilityAINode* CurrentActionNode);

	/**
	 * Array of consideration scores the currently evaluated action node has.
	 * Filled when executing SetConsiderationScore().
	 * @see SetConsiderationScore()
	 */
	UPROPERTY()
	TArray<float> ConsiderationScores;
};
