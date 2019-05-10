#include "SEdNode_UtilityAINode.h"
#include "UtilityAIEditorPCH.h"
#include "Colors_UtilityAI.h"
#include "SLevelOfDetailBranchNode.h"
#include "SInlineEditableTextBlock.h"
#include "SCommentBubble.h"
#include "SlateOptMacros.h"
#include "SGraphPin.h"
#include "GraphEditorSettings.h"
#include "EdNode_UtilityAINode.h"

#define LOCTEXT_NAMESPACE "EdNode_UtilityAI"

//////////////////////////////////////////////////////////////////////////
class SUtilityAIPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SUtilityAIPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);

		bShowLabel = true;

		GraphPinObj = InPin;
		check(GraphPinObj != nullptr);

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);

		SBorder::Construct(SBorder::FArguments()
			.BorderImage(this, &SUtilityAIPin::GetPinBorder)
			.BorderBackgroundColor(this, &SUtilityAIPin::GetPinColor)
			.OnMouseButtonDown(this, &SUtilityAIPin::OnPinMouseDown)
			.Cursor(this, &SUtilityAIPin::GetPinCursor)
			.Padding(FMargin(10.0f))
		);
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return UtilityAIColors::Pin::Default;
	}

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override
	{
		return SNew(STextBlock);
	}

	const FSlateBrush* GetPinBorder() const
	{
		return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Body"));
	}
};


//////////////////////////////////////////////////////////////////////////
void SEdNode_UtilityAINode::Construct(const FArguments& InArgs, UEdNode_UtilityAINode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
	InNode->SEdNode = this;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEdNode_UtilityAINode::UpdateGraphNode()
{
	const FMargin NodePadding = FMargin(2.0f);

	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	OutputPinBox.Reset();

	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0.0f)
			.BorderBackgroundColor(this, &SEdNode_UtilityAINode::GetBorderBackgroundColor)
			[
				SNew(SOverlay)

				// Pins and node details
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)

					// INPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Top)
						[
							SAssignNew(LeftNodeBox, SVerticalBox)
						]
					]

					// STATE NAME AREA
					+ SVerticalBox::Slot()
					.Padding(FMargin(NodePadding.Left, 0.0f, NodePadding.Right, 0.0f))
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(NodeBody, SBorder)
							.BorderImage(FEditorStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
							.BorderBackgroundColor(this, &SEdNode_UtilityAINode::GetBackgroundColor)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							.Visibility(EVisibility::SelfHitTestInvisible)
							[
								SNew(SOverlay)
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											// POPUP ERROR MESSAGE
											SAssignNew(ErrorText, SErrorText)
											.BackgroundColor(this, &SEdNode_UtilityAINode::GetErrorColor)
											.ToolTipText(this, &SEdNode_UtilityAINode::GetErrorMsgToolTip)
										]

										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(SHorizontalBox)
											+ SHorizontalBox::Slot()
											.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
											[
												SNew(SVerticalBox)
												+ SVerticalBox::Slot()
												.AutoHeight()
												[
													SAssignNew(InlineEditableText, SInlineEditableTextBlock)
													.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
													.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
													.OnVerifyTextChanged(this, &SEdNode_UtilityAINode::OnVerifyNameTextChanged)
													.OnTextCommitted(this, &SEdNode_UtilityAINode::OnNameTextCommited)
													.IsReadOnly(this, &SEdNode_UtilityAINode::IsNameReadOnly)
													.IsSelected(this, &SEdNode_UtilityAINode::IsSelectedExclusively)
												]
												+ SVerticalBox::Slot()
												.AutoHeight()
												[
													NodeTitle.ToSharedRef()
												]
											]
										]
									]
								]
							]
						]
					]

					// OUTPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Bottom)
						[
							SAssignNew(RightNodeBox, SVerticalBox)
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.Padding(20.0f, 0.0f)
							.FillHeight(1.0f)
							[
								SAssignNew(OutputPinBox, SHorizontalBox)
							]
						]
					]
				]
			]
		];

	// Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SEdNode_UtilityAINode::CreatePinWidgets()
{
	UEdNode_UtilityAINode* StateNode = CastChecked<UEdNode_UtilityAINode>(GraphNode);

	for (int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SUtilityAIPin, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SEdNode_UtilityAINode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility( TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced) );
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f,0.0f)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		OutputPinBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.0f)
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

bool SEdNode_UtilityAINode::IsNameReadOnly() const
{
	UEdNode_UtilityAINode* EdNode_Node = Cast<UEdNode_UtilityAINode>(GraphNode);
	check(EdNode_Node != nullptr);

	UUtilityAIGraph* Graph = EdNode_Node->UtilityAINode->Graph;
	check(Graph != nullptr);

	return !Graph->bCanRenameNode || SGraphNode::IsNameReadOnly();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SEdNode_UtilityAINode::OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo)
{
	SGraphNode::OnNameTextCommited(InText, CommitInfo);

	UEdNode_UtilityAINode* MyNode = CastChecked<UEdNode_UtilityAINode>(GraphNode);

	if (MyNode != nullptr && MyNode->UtilityAINode != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("UtilityAIEditorRenameNode", "Utility AI Editor: Rename Node"));
		MyNode->Modify();
		MyNode->UtilityAINode->Modify();
		MyNode->UtilityAINode->SetNodeTitle(InText);
		UpdateGraphNode();
	}
}

FSlateColor SEdNode_UtilityAINode::GetBorderBackgroundColor() const
{
	UEdNode_UtilityAINode* MyNode = CastChecked<UEdNode_UtilityAINode>(GraphNode);
	return MyNode ? MyNode->GetBackgroundColor() : UtilityAIColors::NodeBorder::HighlightAbortRange0;
}

FSlateColor SEdNode_UtilityAINode::GetBackgroundColor() const
{
	return UtilityAIColors::NodeBody::Default;
}

EVisibility SEdNode_UtilityAINode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

const FSlateBrush* SEdNode_UtilityAINode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

#undef LOCTEXT_NAMESPACE
