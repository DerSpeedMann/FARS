#include "ImageConvertingModule.h"
#include "ModuleCaller.h"
#include "ImageVisualizer.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <windows.h>
#include <imgui.h>

static std::string fileName = "converted.";

static int selected = 0;
static std::string ImageFileTypes[]{ "bmp", "pbm", "pgm", "ppm", "sr", "ras", "jpeg", "jpg", "jpe", "jp2", "tiff", "tif", "png" };

ImageConvertingModule::ImageConvertingModule()
{
    ModuleName = "ImageConverter";
    OutputFileNames[0] = "opencv2_Matching.";
    OutputFileNames[1] = "opencv2_Enrool.";
    ModuleExecutable = "";
}

void ImageConvertingModule::Render()
{
    ImGui::Text("Select output type:");
    if (ImGui::BeginListBox("##ImageConverter", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (int n = 0; n < IM_ARRAYSIZE(ImageFileTypes); n++)
        {
            const bool is_selected = (selected == n);
            if (ImGui::Selectable(ImageFileTypes[n].c_str(), is_selected))
                selected = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
}

bool ImageConvertingModule::Run(std::string inputFile, bool enrollMode, std::string* out_outputFile)
{
    
    int width, height;
    width = height = 0;
    cv::Mat imageMatrix = ImageVisualizer::LoadImageMatrixFromFile(inputFile.c_str(), cv::IMREAD_GRAYSCALE, &width, &height);
    if (imageMatrix.empty())
    {
        std::cout << "Could not load image file: " + inputFile;
        return false;
    }

    std::string fingerprintPath = ModuleCaller::GetFingerprintsPath();

    std::string outputPath = fingerprintPath + OutputFileNames[enrollMode] + ImageFileTypes[selected];

    if (!ImageVisualizer::SaveImageToFile(outputPath.c_str(), imageMatrix))
    {
        std::cout << "Could not save image file: " + inputFile;
        return false;
    }
    *out_outputFile = outputPath;
    return true;
}
