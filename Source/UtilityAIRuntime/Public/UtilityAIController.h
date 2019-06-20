// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UtilityAIController.generated.h"

class UUtilityAINode;

/**
 * Defines the action score evaluation methods.
 */
UENUM(BlueprintType)
enum class EScoreSelectionMethod : uint8
{
	ScoreSelectionMethod_Highest			UMETA(DisplayName = "Highest"),
	ScoreSelectionMethod_WeightedRandom		UMETA(DisplayName = "Weighted Random"),
	ScoreSelectionMethod_RandomTopN			UMETA(DisplayName = "Random Top n")
};

/**
 * This AIController derived class provides functions to evaluate an Utility AI Graph.
 */
UCLASS()
class UTILITYAIRUNTIME_API AUtilityAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	/**
	 * Sets the consideration scores for the current action node.
	 * @see SetConsidearionScore().
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SetConsiderationScores();

	/**
	 * Define here how the consideration score evaluation is implemented.
	 * Is called in SetConsiderationScores().
	 * 
	 * @param ConsiderationName The name of the current consideration node.
	 * @see SetConsiderationScores().
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UtilityAI")
	void ImplementConsiderationScores(FName ConsiderationName);

	/**
	 * Maps the ValueToEvaluate with the defined bookends to 0 - 1, then gets the corresponding value of the custom response curve.
	 * Stores the calculated consideration score in the node itself and returns it.
	 * Also writes the calculated score into the ConsiderationScores array, which is used by SetActionScore()
	 * Uses the current consideration node to retrieve bookends, the custom response curve and to save calculated score within.
	 * 
	 * @param ValueToEvaluate The value to normalize and evaluate.
	 * @see SetActionScore()
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	float SetConsiderationScore(float ValueToEvaluate);

	/**
	 * Sets the consideration scores for all actions, then calculates all action scores.
	 * 
	 * @param ActionNodes Action nodes to calculate the scores for.
	 * @see SetConsiderationScores().
	 * @see SetActionScore().
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SetActionScores(TArray<UUtilityAINode*> ActionNodes);

	/**
	 * Iterates over the ConsiderationScores, multiplies each score and saves the calculated action score into the current action node.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SetActionScore();

	/**
	 * Sorts given action scores from highest to lowest score.
	 *
	 * @param	ActionScores Scores to sort.
	 * @return	Sorted action scores, descending order.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SortActionScores(TArray<UUtilityAINode*> ActionNodes);

	/**
	 * Selects the next action node based on the selection method.
	 *
	 * @param	ActionNodes		The array of action nodes to select the next action.
	 * @param	SelectionMethod	The method on how to select the action.
	 * @param	TopN			Top n actions to select randomly from. Only matters when "Random Top n" is the SelectionMethod.
	 * @return	The selected action node.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	UUtilityAINode* SelectActionNode(
		TArray<UUtilityAINode*> ActionNodes,
		EScoreSelectionMethod SelectionMethod = EScoreSelectionMethod::ScoreSelectionMethod_Highest,
		int32 TopN = 3);

	/**
	 * Returns the decision node of the given action node.
	 * 
	 * @param WinningActionNode Selected action node to get the decision from.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	UUtilityAINode* GetDecisionFromAction(UUtilityAINode* WinningActionNode) const;

	/** Returns the currently evaluated consideration node. */
	UFUNCTION(BlueprintPure, Category = "UtilityAI")
	FORCEINLINE UUtilityAINode* GetCurrentConsiderationNode() { return CurrentConsiderationNode; }

	/** Returns the currently evaluated action node. */
	UFUNCTION(BlueprintPure, Category = "UtilityAI")
	FORCEINLINE UUtilityAINode* GetCurrentActionNode() { return CurrentActionNode; }

private:
	/**
	 * Array of consideration scores the currently evaluated action node has.
	 * Filled when executing SetConsiderationScore().
	 * @see SetConsiderationScore().
	 */
	UPROPERTY()
	TArray<float> ConsiderationScores;

	/**
	 * Current consideration node.
	 * Set in SetConsiderationScores().
	 */
	UPROPERTY()
	UUtilityAINode* CurrentConsiderationNode;

	/**
	 * Current action node.
	 * Set in SetActionScores().
	 */
	UPROPERTY()
	UUtilityAINode* CurrentActionNode;

	/**
	 * Array of all action scores.
	 * Filled in SortActionScores().
	 */
	UPROPERTY()
	TArray<float> ActionScores;
};
