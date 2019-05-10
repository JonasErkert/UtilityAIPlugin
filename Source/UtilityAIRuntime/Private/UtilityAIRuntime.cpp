#include "UtilityAIRuntimePCH.h"

DEFINE_LOG_CATEGORY(UtilityAIRuntime)

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
}


void FUtilityAIRuntime::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



