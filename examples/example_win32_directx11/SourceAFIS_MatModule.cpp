#include "SourceAFIS_MatModule.h"
#include "ModuleCaller.h"
#include "json.hpp"
#include <string>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <imgui.h>
#include <fstream>

SourceAFIS_MatModule::SourceAFIS_MatModule()
{
    ModuleName = "SourceAFIS Matcher";
    OutputFile = "SourceAFIS_Score.json";
    ModuleExecutable = L"SourceAfis-Tool.exe";
}

void SourceAFIS_MatModule::Render()
{

}

bool SourceAFIS_MatModule::Run(std::string inputFile, std::string* out_outputFile) {

    if (TemplateFile.empty())
    {
        std::cout << printf("Template path is not set!");
        return false;
    }

    std::wstring parameters = L" -m " + ModuleCaller::stringConvert(inputFile) + L" " +
        ModuleCaller::GetTmpPath() + ModuleCaller::stringConvert(TemplateFile) + L" " +
        ModuleCaller::GetTmpPath() + ModuleCaller::stringConvert(OutputFile);
    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
    if (exitCode != 0)
    {
        std::cout << printf("Program closed with error %d!\n", exitCode);
    };
    *out_outputFile = OutputFile;
    return exitCode != 0;
}

double SourceAFIS_MatModule::GetResult()
{
    if (OutputFile.empty())
    {
        std::cout << printf("OutputFile path is not set!");
        return 0;
    }

    std::ifstream f(OutputFile);
    double score = nlohmann::json::parse(f);

    return score;
}
