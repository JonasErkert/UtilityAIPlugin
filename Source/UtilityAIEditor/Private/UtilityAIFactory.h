#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UtilityAIFactory.generated.h"

UCLASS()
class UTILITYAIEDITOR_API UUtilityAIFactory : public UFactory
{
	GENERATED_BODY()

public:
	UUtilityAIFactory();
	virtual ~UUtilityAIFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
