#include "UtilityAIAssetEditor/UtilityAIEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Misc/Paths.h"

TSharedPtr<FSlateStyleSet> FUtilityAIEditorStyle::StyleSet = nullptr;

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

void FUtilityAIEditorStyle::Initialize()
{
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable(new FSlateStyleSet("UtilityAIEditorStyle"));

	StyleSet->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("UtilityAIPlugin/Resources"));
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	StyleSet->Set("UtilityAIEditor.AutoArrange", new IMAGE_BRUSH("AutoArrangeIcon", Icon40x40));
	StyleSet->Set(FName(TEXT("ClassThumbnail.UtilityAIGraph")), new IMAGE_BRUSH("utilityai_64", Icon64x64));
	StyleSet->Set(FName(TEXT("ClassIcon.UtilityAIGraph")), new IMAGE_BRUSH("utilityai_16", Icon16x16));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FUtilityAIEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

const FName& FUtilityAIEditorStyle::GetStyleSetName()
{
	return StyleSet->GetStyleSetName();
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
