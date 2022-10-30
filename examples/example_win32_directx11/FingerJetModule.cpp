#include "FingerJetModule.h"
#include "ModuleCaller.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <imgui.h>

FingerJetModule::FingerJetModule()
{
    ModuleName = "FingerJetFXOSE";
    OutputFile = "fjfx01.ist";
    ModuleExecutable = L"fjfxSample.exe";
}

void FingerJetModule::Render()  
{
    float value = 0.0f;
    int value2 = 0;
    ImGui::DragFloat("Value", &value);
    ImGui::DragInt("Value2", &value2);
}

bool FingerJetModule::Run(std::string inputFile, std::string* out_outputFile) {

    std::wstring parameters = ModuleCaller::stringConvert(inputFile) + L" " + ModuleCaller::GetTmpPath() + ModuleCaller::stringConvert(OutputFile);
    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
    if (exitCode != 0)
    {
        std::cout << printf("Program closed with error %d!\n", exitCode);
    };
    *out_outputFile = OutputFile;
    return exitCode != 0;
}
