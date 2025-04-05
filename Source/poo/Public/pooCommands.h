// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "pooStyle.h"

class FpooCommands : public TCommands<FpooCommands>
{
public:

	FpooCommands()
		: TCommands<FpooCommands>(TEXT("poo"), NSLOCTEXT("Contexts", "poo", "poo Plugin"), NAME_None, FpooStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
	TSharedPtr< FUICommandInfo > HTML5PackageAction;
};
