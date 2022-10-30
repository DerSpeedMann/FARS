#include "ImageConvertingModule.h"
#include "ModuleCaller.h"
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <imgui.h>

ImageConvertingModule::ImageConvertingModule()
{
    ModuleName = "ImageConverter";
    OutputFile = L"";
    ModuleExecutable = L"";
}

void ImageConvertingModule::Render()
{
    static float value = 0.0f;
    static int value2 = 0;
    ImGui::DragFloat("Value", &value);
    ImGui::DragInt("Value2", &value2);
}

bool ImageConvertingModule::Run(std::wstring inputFile)
{
    return true;
}
