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
    ModuleExecutable = L"SourceAfis-Tool.exe";
}

void SourceAFIS_ExtModule::Render()
{

}

bool SourceAFIS_ExtModule::Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath) {

    std::wstring outPath = ModuleCaller::GetTmpPath() + ModuleCaller::stringConvert(OutputFileNames[enrollMode]);
    *out_outputFilePath = ModuleCaller::stringConvert(outPath);
    std::wstring parameters = L" -e " + ModuleCaller::stringConvert(inputFilePath) + L" " + outPath;
    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
    if (exitCode != 0)
    {
        std::cout << printf("Program closed with error %d!\n", exitCode);
    };
    *out_outputFilePath = ModuleCaller::stringConvert(outPath);
    return exitCode != 0;
}
