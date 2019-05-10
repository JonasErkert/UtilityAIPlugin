#pragma once

#include "CoreMinimal.h"
#include "AutoLayoutStrategy.h"
#include "ForceDirectedLayoutStrategy.generated.h"

UCLASS()
class UForceDirectedLayoutStrategy : public UAutoLayoutStrategy
{
	GENERATED_BODY()
public:
	UForceDirectedLayoutStrategy();
	virtual ~UForceDirectedLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) override;

protected:
	virtual FBox2D LayoutOneTree(UUtilityAINode* RootNode, const FBox2D& PreTreeBound);

protected:
	bool bRandomInit;
	float InitTemperature;
	float CoolDownRate;
};
