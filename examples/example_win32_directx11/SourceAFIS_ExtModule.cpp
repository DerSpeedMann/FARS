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
    OutputFile = "SourceAFIS_Template.cbor";
    ModuleExecutable = L"SourceAfis-Tool.exe";
}

void SourceAFIS_ExtModule::Render()
{

}

bool SourceAFIS_ExtModule::Run(std::string inputFile, std::string* out_outputFile) {

    std::wstring parameters = L" -e " + ModuleCaller::stringConvert(inputFile) + L" " + ModuleCaller::GetTmpPath() + ModuleCaller::stringConvert(OutputFile);
    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
    if (exitCode != 0)
    {
        std::cout << printf("Program closed with error %d!\n", exitCode);
    };
    *out_outputFile = OutputFile;
    return exitCode != 0;
}
