#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "TextProperty.h"
#include <Curves/CurveFloat.h>
#include "UtilityAINode.generated.h"

class UUtilityAIGraph;
class UUtilityAIEdge;

/**
 * Defines the different response curve types.
 */
UENUM(BlueprintType)
enum class EResponseCurveType : uint8
{
	ResponseCurveType_Linear		UMETA(DisplayName = "Linear Response Curve"),
	ResponseCurveType_Quadratic		UMETA(DisplayName = "Quadratic Response Curve"),
	ResponseCurveType_Logistic		UMETA(DisplayName = "Logistic Response Curve")
};

/**
 * Defines the node type.
 */
UENUM(BlueprintType)
enum class ENodeType : uint8
{
	NodeType_Root				UMETA(DisplayName = "Root Node"),
	NodeType_Action				UMETA(DisplayName = "Action Node"),
	NodeType_Consideration		UMETA(DisplayName = "Consideration Node"),
	NodeType_Decision			UMETA(DisplayName = "Decision Node")
};

UCLASS(Blueprintable)
class UTILITYAIRUNTIME_API UUtilityAINode : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAINode();
	virtual ~UUtilityAINode();

	UPROPERTY(VisibleDefaultsOnly, Category = "UtilityAINode")
	UUtilityAIGraph* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAINode")
	TArray<UUtilityAINode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAINode")
	TArray<UUtilityAINode*> ChildrenNodes;

	UPROPERTY(BlueprintReadOnly, Category = "UtilityAINode")
	TMap<UUtilityAINode*, UUtilityAIEdge*> Edges;

	/** The name of the node. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAINode")
	FName NodeName;

	/** The node this this node represents. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAINode")
	ENodeType NodeType;

	/**
	 * Disabling the action node will set the action score to 0, so it will never get selected.
	 * Useful for debugging.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UtilityAINode|Action Node")
	bool bDisableAction;

	/** A custom, in editor (based on tangents) defined response curve. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAINode|Consideration Node")
	UCurveFloat* CustomResponseCurve;

	/**
	 * If true, the custom curve has a normalized range (0-1).
	 * Changes evaluation in SetConsiderationScore().
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAINode|Consideration Node")
	bool bIsNormalizedCustomCurve;

	/** The mathematical response curve type. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAINode|Consideration Node")
	EResponseCurveType ResponseCurveType;

	/** Used to determine if node uses the editor curve or the mathematical curve representation. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAINode|Consideration Node")
	bool bUsingCustomResponseCurve;

	/**
	 * The mathematical response curve type parameters IN ORDER:
	 * m = slope
	 * k = vertical size
	 * b = vertical shift
	 * c = horizontal shift
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UtilityAINode|Consideration Node")
	TArray<int32> ResponseCurveParameters;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UtilityAINode|Debug")
	float ActionScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UtilityAINode|Debug")
	float ConsiderationScore;

	/**
	 * The minimum value a certain parameter can have.
	 * All values below will get clamped to the defined value.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UtilityAINode|Consideration Node")
	float BookendMin;

	/**
	 * The maximum value a certain parameter can have.
	 * All values above will get clamped to the defined value.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UtilityAINode|Consideration Node")
	float BookendMax;

	/**
	 * Defines if this node uses additional weight.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UtilityAINode|Action Node")
	bool bUseWeight;
	
	/**
	 * Additional weighting the score is multiplied with.
	 * Used to implicitly prioritize scores.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UtilityAINode|Action Node", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Weight;

	/** If true, the the evaluated score gets inverted (1 - x). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UtilityAINode|Consideration Node")
	bool bInvertScore;

	/**
	 * If true, the associated curve has a random shift on the y-Axis.
	 * Shift depends on the y-Axis range.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UtilityAINode|Consideration Node")
	bool bRandomShift;

	/** Define the range of the random shift. Dependent on the y-Axis range of the curve. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UtilityAINode|Consideration Node")
	float RandomShiftRange;

	/** Saves the calculated random shift. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UtilityAINode|Debug")
	float RandomShift;

	/** Determine of the random shift of this node is already set. */
	bool bIsShiftSet;

	UFUNCTION(BlueprintCallable, Category = "UtilityAINode")
	virtual UUtilityAIEdge* GetEdge(UUtilityAINode* ChildNode);

	UFUNCTION(BlueprintCallable, Category = "UtilityAINode")
	bool IsLeafNode() const;

	UFUNCTION(BlueprintCallable, Category = "UtilityAINode")
	UUtilityAIGraph* GetGraph() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UtilityAINode")
	FText GetDescription() const;
	virtual FText GetDescription_Implementation() const;

	UFUNCTION(BlueprintCallable, Category = "UtilityAINode")
	FName GetNodeName() const;

	//////////////////////////////////////////////////////////////////////////
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "UtilityAINode_Editor")
	FText NodeTitle;

	UPROPERTY(VisibleDefaultsOnly, Category = "UtilityAINode_Editor")
	TSubclassOf<UUtilityAIGraph> CompatibleGraphType;

	UPROPERTY(EditDefaultsOnly, Category = "UtilityAINode_Editor")
	FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category = "UtilityAINode_Editor")
	FText ContextMenuName;
#endif

#if WITH_EDITOR
	virtual FLinearColor GetBackgroundColor() const;

	virtual FText GetNodeTitle() const;

	virtual void SetNodeTitle(const FText& NewTitle);

	virtual bool CanCreateConnection(UUtilityAINode* Other, FText& ErrorMessage);
#endif
};
