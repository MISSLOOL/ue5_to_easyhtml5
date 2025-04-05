// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class SHTML5PackagerWindow;

class FpooModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	
	/** HTML5 packaging function */
	void HTML5PackageButtonClicked();

private:

	void RegisterMenus();
	
	/** Keeps track of the HTML5 packager window */
	TSharedPtr<SWindow> HTML5PackagerWindow;
	TSharedPtr<SHTML5PackagerWindow> HTML5PackagerWidget;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
