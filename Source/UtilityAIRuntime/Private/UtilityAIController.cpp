// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAIController.h"
#include "UtilityAIGraph.h"
#include "UtilityAINode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveFloat.h"


AUtilityAIController::AUtilityAIController()
{
	CurrentConsiderationScore = 0.f;
}

void AUtilityAIController::InitializeUtilityAI(UUtilityAIGraph* UtilityAI)
{
	UUtilityAINode* RootNode = UtilityAI->RootNodes[0];

	if (RootNode->NodeType == ENodeType::NodeType_Root)
	{
		for (auto ActionNode : RootNode->ChildrenNodes)
		{
			ActionNodes.Add(ActionNode);
		}
	}
}

void AUtilityAIController::SetConsiderationScores()
{
	for (auto ActionChildNode : CurrentActionNode->ChildrenNodes)
	{
		if (ActionChildNode->NodeType == ENodeType::NodeType_Consideration)
		{
			// Set current consideration node.
			// Important to be set before the consideration scores are implemented and SetConsiderationScore() is called
			CurrentConsiderationNode = ActionChildNode;

			// How the consideration scores are implemented is implemented in Blueprint
			ImplementConsiderationScores(ActionChildNode->GetNodeName());
		}
	}
}

float AUtilityAIController::SetConsiderationScore(float ValueToEvaluate)
{
	checkf(CurrentConsiderationNode, TEXT("Current consideration node is null!"));

	float MinBookend = CurrentConsiderationNode->BookendMin;
	float MaxBookend = CurrentConsiderationNode->BookendMax;

	UCurveFloat* ResponseCurve = CurrentConsiderationNode->CustomResponseCurve;
	float CalculatedConsiderationScore = 0.f;

	// Make sure the custom response curve is set
	if (ensureMsgf(ResponseCurve, TEXT("Unexpected null custom response curve! The current consideration node has no custom curve assigned!")))
	{
		// If the response curve is normalized...
		if (CurrentConsiderationNode->bIsNormalizedCustomCurve)
		{
			// ... the ValueToEvaluate can be normalized/ remapped
			float NormalizedValue = UKismetMathLibrary::MapRangeClamped(ValueToEvaluate, MinBookend, MaxBookend, 0.f, 1.f);

			// The remapped value is the fed into the response curve
			CalculatedConsiderationScore = ResponseCurve->GetFloatValue(NormalizedValue);
		}
		else
		{
			// If the custom curve isn't normalized, the ValueToEvaluate is probably also not in range 0-1, but matching the custom curves bookends
			// Therefore the value from the curve doesn't need to be normalized/ remapped anymore
			CalculatedConsiderationScore = ResponseCurve->GetFloatValue(ValueToEvaluate);
		}
	}

	// Invert score if ticked in node
	if (CurrentConsiderationNode->bInvertScore)
	{
		CalculatedConsiderationScore = 1 - CalculatedConsiderationScore;
	}

	// Store the consideration score in the node itself
	CurrentConsiderationNode->ConsiderationScore = CalculatedConsiderationScore;

	// Add the score to the ConsiderationScores array, which is later used to set the action score
	ConsiderationScores.Add(CalculatedConsiderationScore);

	CurrentConsiderationScore = CalculatedConsiderationScore;

	return CalculatedConsiderationScore;
}

void AUtilityAIController::SetActionScores()
{
	for (auto ActionNode : ActionNodes)
	{
		CurrentActionNode = ActionNode;

		SetConsiderationScores();
		SetActionScore();
	}
}

void AUtilityAIController::SetActionScore()
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

void AUtilityAIController::SortActionScores()
{
	// Clear array if it contains any elements
	if (ActionScores.Num() > 0)
	{
		ActionScores.Empty();
	}

	for (auto ActionNode : ActionNodes)
	{
		ActionScores.Add(ActionNode->ActionScore);
	}

	TArray<float> InScores = ActionScores;
	TArray<float> SortedScores;

	// Use action score instead of InScores to avoid iterating a changing array
	for (const auto& Score : ActionNodes)
	{
		int32 MaxScoreIndex	= 0;
		float MaxScore		= 0.f;
		UKismetMathLibrary::MaxOfFloatArray(InScores, MaxScoreIndex, MaxScore);

		SortedScores.Add(MaxScore);
		InScores.RemoveAt(MaxScoreIndex);
	}

	ActionScores = SortedScores;
}

UUtilityAINode* AUtilityAIController::SelectActionNode(EScoreSelectionMethod SelectionMethod /*= EScoreSelectionMethod::ScoreSelectionMethod_Highest*/, int32 TopN /*= 3*/)
{
	UUtilityAINode* SelectedAction = nullptr;
	float ScoreToFind = 0.f;

	switch (SelectionMethod)
	{
	case EScoreSelectionMethod::ScoreSelectionMethod_Highest:
		{
			// Just pick the highest score
			ScoreToFind = ActionScores[0];
		}
		break;

	case EScoreSelectionMethod::ScoreSelectionMethod_WeightedRandom:
		{
			// Add scores of all actions
			float Total = 0.f;
			for (auto ActionNode : ActionNodes)
			{
				Total += ActionNode->ActionScore;
			}

			float Choice = (FMath::Rand() * (1.f / RAND_MAX)) * Total;
			float Accumulation			= 0.f;

			for (auto ActionScore : ActionScores)
			{
				Accumulation += ActionScore / Total;

				if (Choice < Accumulation)
				{
					ScoreToFind = ActionScore;
					break;
				}
			}
		}
		break;

		case EScoreSelectionMethod::ScoreSelectionMethod_RandomTopN:
		{
			// Get a random action in the defined range
			int32 RandomIndex = FMath::RandRange(0, TopN);
			ScoreToFind = ActionScores[RandomIndex];
		}
		break;
	}

	// Find the action with the corresponding score
	for (auto ActionNode : ActionNodes)
	{
		if (FMath::IsNearlyEqual(ActionNode->ActionScore, ScoreToFind))
		{
			SelectedAction = ActionNode;
			break;
		}
	}

	return SelectedAction;
}

UUtilityAINode* AUtilityAIController::GetDecisionFromAction(UUtilityAINode* WinningActionNode) const
{
	UUtilityAINode* DecisionNode = nullptr;

	// Search the action nodes' children for the decision node
	for (auto ChildrenNode : WinningActionNode->ChildrenNodes)
	{
		if (ChildrenNode->NodeType == ENodeType::NodeType_Decision)
		{
			DecisionNode = ChildrenNode;
			break;
		}
	}

	checkf(DecisionNode, TEXT("Current action node has no decision node!"));
	return DecisionNode;
}

void AUtilityAIController::RunUtilityAI(EScoreSelectionMethod SelectionMethod /*= EScoreSelectionMethod::ScoreSelectionMethod_Highest*/, int32 TopN /*= 3*/)
{
	if (ActionNodes.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing InitializeUtilityAI function on begin play! Aborting."));
		return;
	}

	SetActionScores();
	SortActionScores();
	UUtilityAINode* SelectedDecision = GetDecisionFromAction(SelectActionNode(SelectionMethod, TopN));

	ImplementDecisions(SelectedDecision->GetNodeName());
}
