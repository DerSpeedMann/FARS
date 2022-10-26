#include "Module.h"
#include "ModuleCaller.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <imgui.h>

    Module::Module(std::string moduleName, std::wstring moduleExecutable, std::wstring outputFileName) : ModuleName(moduleName), ModuleExecutable(moduleExecutable)
    {
        SetOutputFile(outputFileName);
    }
    Module::~Module()
    {

    }
    bool Module::Run(std::wstring inputFile)
    {
        std::wstring parameters = inputFile + L" " + ModuleCaller::GetTmpPath() + OutputFile;
        int exitCode = ModuleCaller::CallModule(ModuleCaller::GetModulePath() + ModuleExecutable, parameters);
        if (exitCode != 0)
        {
            std::cout << printf("Program closed with error %d!\n", exitCode);
        };
        return exitCode != 0;
    }
    void Module::Render()
    {
        static float value = 0.0f;
        static int value2 = 0;
        ImGui::DragFloat("Value", &value);
        ImGui::DragInt("Value2", &value2);
    }
    void Module::SetOutputFile(std::wstring fileName)
    {
        // TODO: check if file exists
        OutputFile = fileName;
    }
    std::wstring Module::GetOutputFile()
    {
        return OutputFile;
    }
    std::string Module::GetModuleName()
    {
        std::string name = "##";
        if (!ModuleName.empty())
        {
            name = ModuleName;
        }

        return name;
    }

