#include "UtilityAIRuntimePCH.h"

DEFINE_LOG_CATEGORY(UtilityAIRuntime)

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "UtilityAIGameplayDebugger.h"
#endif

class FUtilityAIRuntime : public IUtilityAIRuntime
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FUtilityAIRuntime, UtilityAIRuntime )



void FUtilityAIRuntime::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	UE_LOG(UtilityAIRuntime, Warning, TEXT("UtilityAIRuntime module has started!"));
 
#if WITH_GAMEPLAY_DEBUGGER
	// If the gameplay debugger is available, register the category and notify the editor about the changes
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();

	GameplayDebuggerModule.RegisterCategory("UtilityAI", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_UtilityAI::MakeInstance), EGameplayDebuggerCategoryState::EnabledInGameAndSimulate);

	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}


void FUtilityAIRuntime::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(UtilityAIRuntime, Warning, TEXT("UtilityAIRuntime module has shut down!"));
 
#if WITH_GAMEPLAY_DEBUGGER
	// If the gameplay debugger is available, unregister the category
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("UtilityAI");
	}
#endif
}



