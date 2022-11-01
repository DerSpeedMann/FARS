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
    OutputFileName = "SourceAFIS_Score.json";
    ModuleExecutable = L"SourceAfis-Tool.exe";
}

void SourceAFIS_MatModule::Render()
{

}

bool SourceAFIS_MatModule::Run(std::string inputFilePath, std::string enrolledTemplatePath, std::string* out_outputFilePath) {

    std::wstring outPath = ModuleCaller::GetTmpPath() + ModuleCaller::stringConvert(OutputFileName);

    std::wstring parameters = L" -m " + ModuleCaller::stringConvert(inputFilePath) + L" " +
                                ModuleCaller::stringConvert(enrolledTemplatePath) + L" " + outPath;

    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
    if (exitCode != 0)
    {
        std::cout << printf("Program closed with error %d!\n", exitCode);
    };

    *out_outputFilePath = ModuleCaller::stringConvert(outPath);
    return exitCode != 0;
}

double SourceAFIS_MatModule::GetResult()
{
    std::ifstream f(ModuleCaller::stringConvert(ModuleCaller::GetTmpPath()) + OutputFileName);
    double score = nlohmann::json::parse(f);
    return score;
}
