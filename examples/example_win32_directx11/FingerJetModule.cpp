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
    OutputFileNames[1] = "fjfx01_Enroll.ist";
    ModuleExecutable = "fjfxSample.exe";
}

void FingerJetModule::Render()  
{

}

bool FingerJetModule::Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath, ErrorHandler *errorHandler) {

    std::string outPath = ModuleCaller::GetTmpPath() + OutputFileNames[enrollMode];
   
    std::string parameters = inputFilePath + " " + outPath;
    int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);

    *out_outputFilePath = outPath;

    if (exitCode != 0)
    {
        std::string errorMessage;
        switch (exitCode) {
        case -1:
            errorMessage = "Could not create Prozess, or missing Module! Check console for more details!";
            break;
        case 9:
            errorMessage = "Cannot open image file: \n" + inputFilePath;
            break;
        case 10:
            errorMessage = "Image Format is not supported (FingerJet only supports .pgm): \n" + inputFilePath;
            break;
        case 11:
            errorMessage = "Image file is too short: \n" + inputFilePath;
            break;
        case 12:
            errorMessage = "Could not allocate immage buffer!";
            break;
        case 14:
        case 15:
            errorMessage = "Could not create template file: \n" + outPath;
            break;
        default:
            errorMessage = "Unknown Error, check console for more details!";
            break;
        }
        errorHandler->setModuleError(exitCode, errorMessage, ModuleName);
    }

    return exitCode != 0;
}
