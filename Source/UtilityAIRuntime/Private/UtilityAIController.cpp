// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAIController.h"
#include "UtilityAINode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveFloat.h"


TArray<float> AUtilityAIController::SortActionScores(const TArray<float>& ActionScores)
{
	TArray<float> InScores = ActionScores;
	TArray<float> SortedScores;

	// Use action score instead of InScores to avoid iterating a changing array
	for (const auto& Score : ActionScores)
	{
		int32 MaxScoreIndex	= 0;
		float MaxScore		= 0.f;
		UKismetMathLibrary::MaxOfFloatArray(InScores, MaxScoreIndex, MaxScore);

		SortedScores.Add(MaxScore);
		InScores.RemoveAt(MaxScoreIndex);
	}

	return SortedScores;
}

float AUtilityAIController::SetConsiderationScore(UUtilityAINode* CurrentConsiderationNode, float ValueToNormalize)
{
	float MinBookend = CurrentConsiderationNode->BookendMin;
	float MaxBookend = CurrentConsiderationNode->BookendMax;

	float NormalizedValue = UKismetMathLibrary::MapRangeClamped(ValueToNormalize, MinBookend, MaxBookend, 0.f, 1.f);

	UCurveFloat* ResponseCurve	= CurrentConsiderationNode->CustomResponseCurve;
	float ResponseCurveValue	= ResponseCurve->GetFloatValue(NormalizedValue);

	// Store the consideration score in the node itself
	CurrentConsiderationNode->ConsiderationScore = ResponseCurveValue;

	// Add the score to the ConsiderationScores array, which is later used to set the action score
	ConsiderationScores.Add(ResponseCurveValue);

	return ResponseCurveValue;
}

void AUtilityAIController::SetActionScore(UUtilityAINode* CurrentActionNode)
{
	// Ensure the action node has a score of 0 (and no previously evaluated score)
	CurrentActionNode->ActionScore		= 0.f;
	float AccumulatedConsiderationScore	= 1.f; // Initialized with 1 to make the first consideration score multiplication not altering the value

	for (float ConsiderationScore : ConsiderationScores)
	{
		AccumulatedConsiderationScore *= ConsiderationScore;
	}

	// Store the action score in the node itself
	CurrentActionNode->ActionScore = AccumulatedConsiderationScore;

	// Clear the ConsiderationScores array for the next action score node calculation
	ConsiderationScores.Empty();
}
