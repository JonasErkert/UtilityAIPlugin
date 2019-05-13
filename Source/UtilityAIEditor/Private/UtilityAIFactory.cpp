#include "UtilityAIFactory.h"
#include "UtilityAIGraph.h"

#define LOCTEXT_NAMESPACE "UtilityAIFactory"

UUtilityAIFactory::UUtilityAIFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UUtilityAIGraph::StaticClass();
}

UUtilityAIFactory::~UUtilityAIFactory()
{

}

UObject* UUtilityAIFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}

FText UUtilityAIFactory::GetDisplayName() const
{
	return LOCTEXT("UtilityAIText", "UtilityAI");
}

FString UUtilityAIFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewUtilityAI"));
}

#undef LOCTEXT_NAMESPACE
