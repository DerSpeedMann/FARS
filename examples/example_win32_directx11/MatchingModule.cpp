#include "MatchingModule.h"
#include <imgui.h>

bool MatchingModule::Run(std::string inputFilePath, std::string enrolledTemplatePath, std::string* out_outputFilePath)
{
    return true;
}
void MatchingModule::Render()
{
    ImGui::Text("Generic Details");
}

double MatchingModule::GetResult()
{
    return 0;
}

std::string MatchingModule::GetModuleName()
{
    std::string name = "##";
    if (!ModuleName.empty())
    {
        name = ModuleName;
    }

    return name;
}
