// Copyright Epic Games, Inc. All Rights Reserved.

#include "pooCommands.h"

#define LOCTEXT_NAMESPACE "FpooModule"

void FpooCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "poo", "Execute poo action", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(HTML5PackageAction, "HTML5 Package", "Package your project for HTML5", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
