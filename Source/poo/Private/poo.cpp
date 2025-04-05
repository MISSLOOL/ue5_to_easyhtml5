// Copyright Epic Games, Inc. All Rights Reserved.

#include "poo.h"
#include "pooStyle.h"
#include "pooCommands.h"
#include "HTML5PackagerWindow.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Framework/Application/SlateApplication.h"

static const FName pooTabName("poo");

#define LOCTEXT_NAMESPACE "FpooModule"

void FpooModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FpooStyle::Initialize();
	FpooStyle::ReloadTextures();

	FpooCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);
	
		
	PluginCommands->MapAction(
		FpooCommands::Get().HTML5PackageAction,
		FExecuteAction::CreateRaw(this, &FpooModule::HTML5PackageButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FpooModule::RegisterMenus));
}

void FpooModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FpooStyle::Shutdown();

	FpooCommands::Unregister();
}

void FpooModule::HTML5PackageButtonClicked()
{
	// Close existing window if it exists
	if (HTML5PackagerWindow.IsValid())
	{
		HTML5PackagerWindow->RequestDestroyWindow();
		HTML5PackagerWindow.Reset();
		HTML5PackagerWidget.Reset();
	}
	
	// Create a new window
	HTML5PackagerWindow = SNew(SWindow)
		.Title(LOCTEXT("HTML5PackagerWindowTitle", "HTML5 Packager"))
		.SizingRule(ESizingRule::Autosized)
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.HasCloseButton(true)
		.SupportsMaximize(false)
		.SupportsMinimize(false);
		
	// Create the packager widget and add it to the window
	HTML5PackagerWindow->SetContent(
		SAssignNew(HTML5PackagerWidget, SHTML5PackagerWindow)
	);
	
	// Show the window
	FSlateApplication::Get().AddWindow(HTML5PackagerWindow.ToSharedRef());
}

void FpooModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	
	// Add HTML5 Package button to the toolbar
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FpooCommands::Get().HTML5PackageAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
	
	// Add HTML5 Package item to the Window menu
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FpooCommands::Get().HTML5PackageAction, PluginCommands);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FpooModule, poo)