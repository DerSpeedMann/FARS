#include "SourceAFIS_ExtModule.h"
#include "ModuleCaller.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <imgui.h>

SourceAFIS_ExtModule::SourceAFIS_ExtModule()
{
    ModuleName = "SourceAFIS Extractor";
    OutputFileNames[0] = "SourceAFIS_Matching_Template.cbor";
    OutputFileNames[1] = "SourceAFIS_Enrolled_Template.cbor";
    ModuleExecutable = "SourceAfis-Tool.exe";
}

void SourceAFIS_ExtModule::Render()
{

}

bool SourceAFIS_ExtModule::Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath, ErrorHandler* errorHandler) {

    std::string outPath = ModuleCaller::GetTmpPath() + OutputFileNames[enrollMode];

    std::string parameters = " -e " + inputFilePath + " " + outPath;
    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
    if (exitCode != 0)
    {
        std::cout << printf("Program closed with error %d!\n", exitCode);
    };
    *out_outputFilePath = outPath;
    return exitCode != 0;
}
