// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrainComponent.h"
#include "UtilityAIComponent.generated.h"

class UUtilityAIGraph;
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
 * 
 */
UCLASS()
class UTILITYAIRUNTIME_API UUtilityAIComponent : public UBrainComponent
{
	GENERATED_BODY()

	// Begin UBrainComponent overrides
	virtual void RestartLogic() override;
	virtual void StopLogic(const FString& Reason) override;
	virtual void PauseLogic(const FString& Reason) override;
	virtual EAILogicResuming::Type ResumeLogic(const FString& Reason) override;

public:
	virtual bool IsRunning() const override;
	virtual bool IsPaused() const override;
	virtual void Cleanup() override;
	// End UBrainComponent overrides

	// Begin UActorComponent overrides
	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End UActorComponent overrides

public:
	UUtilityAIComponent();
	
	// UBrainComponent necessary implementation
	void RestartUtilityAI();
	void StopUtilityAI();
	void PauseUtilityAI();
	void ResumeUtilityAI();

	/**
	 * Initialize the Utility AI.
	 *
	 * @param UtilityAI The utility ai graph which defines this ai.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void InitializeUtilityAI(UUtilityAIGraph* UtilityAI);

	/**
	 * Sets the consideration scores for the current action node.
	 * @see SetConsidearionScore().
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SetConsiderationScores();

	/**
	 * Define here how the consideration score evaluation is implemented.
	 *
	 * @param	ConsiderationName The name of the current consideration node.
	 * @see		SetConsiderationScores().
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UtilityAI")
	bool ImplementConsiderationScores(FName ConsiderationName);

	/**
	 * Maps the ValueToEvaluate with the defined bookends to 0 - 1, then gets the corresponding value of the custom response curve.
	 * Stores the calculated consideration score in the node itself and returns it.
	 * Also writes the calculated score into the ConsiderationScores array, which is used by SetActionScore()
	 * Uses the current consideration node to retrieve bookends, the custom response curve and to save calculated score within.
	 *
	 * @param	ValueToEvaluate The value to normalize and evaluate.
	 * @see		SetActionScore()
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	float SetConsiderationScore(float ValueToEvaluate);

	/**
	 * Sets the consideration scores for all actions, then calculates all action scores.
	 *
	 * @see SetConsiderationScores().
	 * @see SetActionScore().
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SetActionScores();

	/**
	 * Iterates over the ConsiderationScores, multiplies each score and saves the calculated action score into the current action node.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SetActionScore();

	/**
	 * Sorts action scores from highest to lowest score.
	 *
	 * @return Sorted action scores, descending order.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void SortActionScores();

	/**
	 * Selects the next action node based on the selection method.
	 *
	 * @param	SelectionMethod	The method on how to select the action.
	 * @param	TopN			Top n actions to select randomly from. Only matters when "Random Top n" is the SelectionMethod.
	 * @return	The selected action node.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	UUtilityAINode* SelectActionNode(EScoreSelectionMethod SelectionMethod = EScoreSelectionMethod::ScoreSelectionMethod_Highest, int32 TopN = 3);

	/**
	 * Returns the decision node of the given action node.
	 *
	 * @param WinningActionNode Selected action node to get the decision from.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	UUtilityAINode* GetDecisionFromAction(UUtilityAINode* WinningActionNode) const;

	/**
	 * Define here how the decisions are implemented.
	 *
	 * @param	DecisionName The name of the selected decision node.
	 * @see		RunUtilityAI().
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UtilityAI")
	void ImplementDecisions(FName DecisionName);

	/**
	 * Check if the decision has changed since the last evaluation.
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	bool HasDecisionChanged(UUtilityAINode* CurrentDecision);

	/**
	 * Is called when a decision has changed.
	 *
	 * @param NewDecision The newly selected decision.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UtilityAI")
	void DecisionChanged(UUtilityAINode* NewDecision);

	/**
	 * Evaluates the utility ai graph.
	 * Sets consideration and action scores, selects an action based on the selection method and then chooses the appropriate decision node.
	 * Needs ImplementConsiderationScores() and ImplementDecision() implementations to work.
	 * If the decision has changed since the last evaluation, ImplementDecision() is called.
	 *
	 * @param	SelectionMethod	The method on how to select the action.
	 * @param	TopN			Top n actions to select randomly from. Only matters when "Random Top n" is the SelectionMethod.
	 * @return	The selected action.
	 * @see		ImplementConsiderationScores().
	 * @see		ImplementDecision().
	 * @see		HasDecisionChanged().
	 */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	UUtilityAINode* ExecuteUtilityAI(EScoreSelectionMethod SelectionMethod = EScoreSelectionMethod::ScoreSelectionMethod_Highest, int32 TopN = 3);

	/** Returns the action nodes of the assigned utility ai graph. */
	UFUNCTION(BlueprintPure, Category = "UtilityAI")
	FORCEINLINE TArray<UUtilityAINode*> GetActionNodes() { return ActionNodes; }

	/** Returns the currently evaluated consideration node. */
	UFUNCTION(BlueprintPure, Category = "UtilityAI")
	FORCEINLINE UUtilityAINode* GetCurrentConsiderationNode() { return CurrentConsiderationNode; }

	/** Returns the currently evaluated action node. */
	UFUNCTION(BlueprintPure, Category = "UtilityAI")
	FORCEINLINE UUtilityAINode* GetCurrentActionNode() { return CurrentActionNode; }

	/** Returns the consideration score of the currently evaluated consideration. */
	UFUNCTION(BlueprintPure, Category = "UtilityAI")
	FORCEINLINE float GetConsiderationScore() { return CurrentConsiderationScore; }

private:
	/**
	 * Array of action nodes the assigned graph has.
	 * Filled in InitilizeUtilityAI() and used in RunUtilityAI().
	 */
	UPROPERTY()
	TArray<UUtilityAINode*> ActionNodes;

	/**
	 * Array of consideration scores the currently evaluated action node has.
	 * Filled when executing SetConsiderationScore().
	 * @see SetConsiderationScore().
	 */
	UPROPERTY()
	TArray<float> ConsiderationScores;

	/**
	 * Consideration score of the currently evaluated consideration.
	 * Useful/ intended for debugging.
	 */
	float CurrentConsiderationScore;

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

	/**
	 * The last chosen decision.
	 */
	UPROPERTY()
	UUtilityAINode* LastDecision;
};
