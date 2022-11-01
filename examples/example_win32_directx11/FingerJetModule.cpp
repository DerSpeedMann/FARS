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
    OutputFileNames[0] = "fjfx01_Matching.ist";
    OutputFileNames[0] = "fjfx01_Enroll.ist";
    ModuleExecutable = "fjfxSample.exe";
}

void FingerJetModule::Render()  
{

}

bool FingerJetModule::Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath) {

    std::string outPath = ModuleCaller::GetTmpPath() + OutputFileNames[enrollMode];
   
    std::string parameters = inputFilePath + " " + outPath;
    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
    if (exitCode != 0)
    {
        std::cout << printf("Program closed with error %d!\n", exitCode);
    };
    *out_outputFilePath = outPath;
    return exitCode != 0;
}
