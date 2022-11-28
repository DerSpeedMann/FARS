#include "SourceAFIS_TemplateConverter.h"
#include "ModuleCaller.h"
#include <iostream>

SourceAFIS_TemplateConverter::SourceAFIS_TemplateConverter()
{
    ModuleName = "Template Converter";
    OutputFileNames[0] = "Converted_Matching_Template";
    OutputFileNames[1] = "Converted_Enrolled_Template";
    ModuleExecutable = "SourceAfis-java-Tool-1.0-SNAPSHOT.jar";
}

void SourceAFIS_TemplateConverter::Render()
{

}

bool SourceAFIS_TemplateConverter::Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath) {

    std::string outPath = ModuleCaller::GetTmpPath() + OutputFileNames[enrollMode];
    int exitCode = 0;

    //TODO: implement
    
    *out_outputFilePath = outPath;
    return exitCode != 0;
}
