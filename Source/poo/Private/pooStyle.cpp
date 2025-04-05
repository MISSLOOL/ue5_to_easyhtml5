// Copyright Epic Games, Inc. All Rights Reserved.

#include "pooStyle.h"
#include "poo.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FpooStyle::StyleInstance = nullptr;

void FpooStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FpooStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FpooStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("pooStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FpooStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("pooStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("poo")->GetBaseDir() / TEXT("Resources"));

	Style->Set("poo.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	Style->Set("poo.HTML5PackageAction", new IMAGE_BRUSH_SVG(TEXT("HTML5Icon"), Icon20x20));
	
	return Style;
}

void FpooStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FpooStyle::Get()
{
	return *StyleInstance;
}
