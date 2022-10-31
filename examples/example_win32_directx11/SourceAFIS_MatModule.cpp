#include "SourceAFIS_MatModule.h"
#include "ModuleCaller.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <imgui.h>

SourceAFIS_MatModule::SourceAFIS_MatModule()
{
    ModuleName = "SourceAFIS Matcher";
    OutputFile = "SourceAFIS_Template.cbor";
    ModuleExecutable = L"SourceAfis-Tool.exe";
}

void SourceAFIS_MatModule::Render()
{

}

bool SourceAFIS_MatModule::Run(std::string inputFile, std::string* out_outputFile) {

    std::wstring parameters = L" -m " + ModuleCaller::stringConvert(inputFile) + L" " + ModuleCaller::GetTmpPath() + ModuleCaller::stringConvert(OutputFile);
    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
    if (exitCode != 0)
    {
        std::cout << printf("Program closed with error %d!\n", exitCode);
    };
    *out_outputFile = OutputFile;
    return exitCode != 0;
}
