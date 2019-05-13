#pragma once

#include "CoreMinimal.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebuggerCategory.h"

//Forward declarations
class AActor;
class APlayerController;
class FGameplayDebuggerCanvasContext;

//The data we're going to print inside the viewport
struct FDataToPrint
{
	float Score;
	float Key;
};

//The class of our custom gameplay debugger category
class FGameplayDebuggerCategory_UtilityAI : public FGameplayDebuggerCategory
{
protected:
	
	//The data that we're going to print
	FDataToPrint Data;

public:
	FGameplayDebuggerCategory_UtilityAI();
 
	/** Creates an instance of this category - will be used on module startup to include our category in the Editor */
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();
 
	/** Collects the data we would like to print */
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
 
	/** Displays the data we collected in the CollectData function */
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;
};
#endif