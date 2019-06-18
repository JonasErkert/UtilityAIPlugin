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

float AUtilityAIController::SetConsiderationScore(UUtilityAINode* CurrentConsiderationNode, float ValueToEvaluate)
{
	float MinBookend = CurrentConsiderationNode->BookendMin;
	float MaxBookend = CurrentConsiderationNode->BookendMax;

	UCurveFloat* ResponseCurve = CurrentConsiderationNode->CustomResponseCurve;
	float ConsiderationScore = 0.f;

	// Make sure the custom response curve is set
	if (ensureMsgf(ResponseCurve, TEXT("Unexpected null custom response curve! The current consideration node has no custom curve assigned!")))
	{
		// If the response curve is normalized...
		if (CurrentConsiderationNode->bIsNormalizedCustomCurve)
		{
			// ... the ValueToEvaluate can be normalized/ remapped
			float NormalizedValue = UKismetMathLibrary::MapRangeClamped(ValueToEvaluate, MinBookend, MaxBookend, 0.f, 1.f);

			// The remapped value is the fed into the response curve
			ConsiderationScore = ResponseCurve->GetFloatValue(NormalizedValue);
		}
		else
		{
			// If the custom curve isn't normalized, the ValueToEvaluate is probably also not in range 0-1, but matching the custom curves bookends
			// Therefore the value from the curve doesn't need to be normalized/ remapped anymore
			ConsiderationScore = ResponseCurve->GetFloatValue(ValueToEvaluate);
		}
	}

	// Invert score if ticked in node
	if (CurrentConsiderationNode->bInvertScore)
	{
		ConsiderationScore = 1 - ConsiderationScore;
	}

	// Store the consideration score in the node itself
	CurrentConsiderationNode->ConsiderationScore = ConsiderationScore;

	// Add the score to the ConsiderationScores array, which is later used to set the action score
	ConsiderationScores.Add(ConsiderationScore);

	return ConsiderationScore;
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
