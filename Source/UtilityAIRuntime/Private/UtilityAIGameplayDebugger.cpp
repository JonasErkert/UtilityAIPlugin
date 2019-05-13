#include "UtilityAIGameplayDebugger.h"
//#include "RPGCharacterBase.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "DebugRenderSceneProxy.h"

FGameplayDebuggerCategory_UtilityAI::FGameplayDebuggerCategory_UtilityAI()
{
	bShowOnlyWithDebugActor = false;
	Data.Score = 33.3f;
	Data.Key = 12.5f;
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_UtilityAI::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_UtilityAI());
}

void FGameplayDebuggerCategory_UtilityAI::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	if (DebugActor)
	{
		// Store the data to our struct
		Data.Score = 99.9f;
		Data.Key = 24.5f;

		//ARPGCharacterBase* Char = Cast<ARPGCharacterBase>(DebugActor);

		//if (Char)
		//{
		//}
	}
}

void FGameplayDebuggerCategory_UtilityAI::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	//Test print with white text
	CanvasContext.Printf(TEXT("Test Print"));
 
	CanvasContext.Printf(FColor::Yellow, TEXT("Yet again another print!"));
 
	//Print the health data with green color
	CanvasContext.Printf(TEXT("{green}Score: %s"), *FString::SanitizeFloat(Data.Score));
 
	//Print the damage data with red color
	CanvasContext.Printf(TEXT("{red}Key: %s"), *FString::SanitizeFloat(Data.Key));
}
#endif
