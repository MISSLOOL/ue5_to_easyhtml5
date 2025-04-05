#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"

class SHTML5PackagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHTML5PackagerWindow) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FReply OnPackageButtonClicked();
	bool ExecuteHTML5Packaging();

	TSharedPtr<SEditableTextBox> OutputPathTextBox;
	TSharedPtr<SEditableTextBox> ProjectPathTextBox;
	FString GetOutputPath() const;
	FString GetProjectPath() const;
}; 