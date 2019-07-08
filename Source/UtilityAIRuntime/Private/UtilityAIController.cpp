// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAIController.h"
#include "UtilityAIGraph.h"

AUtilityAIController::AUtilityAIController()
{
}

UUtilityAIComponent* AUtilityAIController::RunUtilityAI(UUtilityAIGraph* UtilityAI, EScoreSelectionMethod SelectionMethod /*= EScoreSelectionMethod::ScoreSelectionMethod_Highest*/, int32 TopN /*= 3*/)
{
	if (UtilityAI == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunUtilityAI: Unable to run NULL UtilityAI!"));
		return nullptr;
	}

	UUtilityAIComponent* UtilityAIComp = Cast<UUtilityAIComponent>(BrainComponent);

	if (UtilityAIComp == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("RunUtilityAI: Spawning UtilityAIComponent..."));

		UtilityAIComp = NewObject<UUtilityAIComponent>(this, TEXT("UAIComponent"));
		UtilityAIComp->RegisterComponent();
	}

	// Make sure the BrainComponent points to the newly created UAI component
	BrainComponent = UtilityAIComp;

	checkf(UtilityAIComp, TEXT("UtilityAIComponent is NULL!"));
	
	UtilityAIComp->InitializeUtilityAI(UtilityAI);
	UtilityAIComp->ExecuteUtilityAI(SelectionMethod, TopN);

	return UtilityAIComp;
}
