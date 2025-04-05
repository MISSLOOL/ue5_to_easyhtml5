#include "HTML5PackagerWindow.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/Paths.h"
#include "Framework/Application/SlateApplication.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "HTML5LaunchHelper.h"

#define LOCTEXT_NAMESPACE "HTML5Packager"

void SHTML5PackagerWindow::Construct(const FArguments& InArgs)
{
    const float InputBoxWidth = 400.0f;

    // Get the default project path
    FString DefaultProjectPath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
    
    // Get the default output path
    FString DefaultOutputPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("HTML5")));
    
    ChildSlot
    [
        SNew(SBorder)
        .BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
        .Padding(FMargin(8.0f, 4.0f))
        [
            SNew(SVerticalBox)
            
            // Title
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 0, 0, 10)
            [
                SNew(STextBlock)
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                .Text(LOCTEXT("WindowTitle", "HTML5 Packager"))
            ]
            
            // Project path selection
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 4)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(0, 0, 10, 0)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("ProjectPath", "Project File:"))
                    .MinDesiredWidth(100)
                ]
                + SHorizontalBox::Slot()
                [
                    SAssignNew(ProjectPathTextBox, SEditableTextBox)
                    .Text(FText::FromString(DefaultProjectPath))
                    .MinDesiredWidth(InputBoxWidth)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(4, 0, 0, 0)
                [
                    SNew(SButton)
                    .Text(LOCTEXT("Browse", "Browse..."))
                    .OnClicked_Lambda([this]() {
                        IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
                        if (DesktopPlatform)
                        {
                            FString CurrentProjectPath = GetProjectPath();
                            TArray<FString> OutFiles;
                            
                            const FString FileTypes = TEXT("UE4 Project Files (*.uproject)|*.uproject");
                            const bool bOpened = DesktopPlatform->OpenFileDialog(
                                FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
                                LOCTEXT("OpenProjectDialogTitle", "Choose a project file").ToString(),
                                FPaths::GetPath(CurrentProjectPath),
                                TEXT(""),
                                FileTypes,
                                EFileDialogFlags::None,
                                OutFiles
                            );
                            
                            if (bOpened && OutFiles.Num() > 0)
                            {
                                ProjectPathTextBox->SetText(FText::FromString(OutFiles[0]));
                            }
                        }
                        
                        return FReply::Handled();
                    })
                ]
            ]
            
            // Output directory selection
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 4)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(0, 0, 10, 0)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("OutputPath", "Output Directory:"))
                    .MinDesiredWidth(100)
                ]
                + SHorizontalBox::Slot()
                [
                    SAssignNew(OutputPathTextBox, SEditableTextBox)
                    .Text(FText::FromString(DefaultOutputPath))
                    .MinDesiredWidth(InputBoxWidth)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(4, 0, 0, 0)
                [
                    SNew(SButton)
                    .Text(LOCTEXT("Browse", "Browse..."))
                    .OnClicked_Lambda([this]() {
                        IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
                        if (DesktopPlatform)
                        {
                            FString CurrentPath = GetOutputPath();
                            const bool bOpened = DesktopPlatform->OpenDirectoryDialog(
                                FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
                                LOCTEXT("SelectOutputDirectoryTitle", "Select HTML5 Output Directory").ToString(),
                                CurrentPath.IsEmpty() ? FPaths::ProjectSavedDir() : CurrentPath,
                                CurrentPath
                            );
                            
                            if (bOpened)
                            {
                                OutputPathTextBox->SetText(FText::FromString(CurrentPath));
                            }
                        }
                        
                        return FReply::Handled();
                    })
                ]
            ]
            
            // Bottom buttons
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 20, 0, 0)
            .HAlign(HAlign_Right)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SButton)
                    .Text(LOCTEXT("Package", "Package HTML5"))
                    .HAlign(HAlign_Center)
                    .OnClicked(this, &SHTML5PackagerWindow::OnPackageButtonClicked)
                ]
            ]
        ]
    ];
}

FReply SHTML5PackagerWindow::OnPackageButtonClicked()
{
    if (ExecuteHTML5Packaging())
    {
        return FReply::Handled();
    }
    
    return FReply::Handled();
}

bool SHTML5PackagerWindow::ExecuteHTML5Packaging()
{
    const FString ProjectPath = GetProjectPath();
    const FString OutputPath = GetOutputPath();
    
    if (ProjectPath.IsEmpty() || OutputPath.IsEmpty())
    {
        // Show error dialog
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("InvalidPathsError", "Please specify valid project and output paths."));
        return false;
    }
    
    // Use the HTML5 launch helper to package the content
    bool bSuccess = FHTML5LaunchHelper::PackageHTML5(ProjectPath, OutputPath);
    
    if (!bSuccess)
    {
        // Show error dialog
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("PackageError", "Error packaging HTML5 content. Check the log for details."));
        return false;
    }
    
    // Show success dialog
    FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("PackageSuccess", "HTML5 packaging completed successfully!"));
    return true;
}

FString SHTML5PackagerWindow::GetOutputPath() const
{
    return OutputPathTextBox.IsValid() ? OutputPathTextBox->GetText().ToString() : FString();
}

FString SHTML5PackagerWindow::GetProjectPath() const
{
    return ProjectPathTextBox.IsValid() ? ProjectPathTextBox->GetText().ToString() : FString();
}

#undef LOCTEXT_NAMESPACE 