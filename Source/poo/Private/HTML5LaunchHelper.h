#pragma once

#include "CoreMinimal.h"

/**
 * Helper class for packaging HTML5 content
 */
class FHTML5LaunchHelper
{
public:
    /**
     * Packages a project for HTML5 platform
     * 
     * @param ProjectPath - Path to the project file
     * @param OutputPath - Path where the packaged HTML5 content will be stored
     * @param bFullRebuild - Whether to do a full rebuild or incremental
     * @return true if packaging was successful
     */
    static bool PackageHTML5(const FString& ProjectPath, const FString& OutputPath, bool bFullRebuild = true);
    
private:
    /**
     * Generates a simple HTML template for the packaged content
     */
    static bool GenerateHTML5Template(const FString& OutputPath, const FString& ProjectName);
}; 