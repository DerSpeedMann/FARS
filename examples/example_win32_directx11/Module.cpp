#include "Module.h"
#include "ModuleCaller.h"
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <imgui.h>

    Module::Module()
    {

    }
    Module::~Module()
    {

    }

    void Module::Render()
    {
        ImGui::Text("Generic Details");
    }
    bool Module::Run(std::string inputFile, bool enrollMode, std::string* out_outputFile)
    {
        *out_outputFile = "";
        return true;
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

