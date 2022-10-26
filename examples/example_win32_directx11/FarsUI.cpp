#include "FarsUI.h"
#include "imgui.h"
#include "ModuleCaller.h"
#include "Module.h"
#include "ImageVisualiser.h"
#include "imgui_impl_dx11.h"
#include <locale>
#include <codecvt>
#include <string>
#include <iostream>
#include <d3d11.h>

namespace FarsUI
{
    std::wstring InputImage = ModuleCaller::GetFingerprintsPath() + L"a001_03p.pgm";

    int my_image_width = 0;
    int my_image_height = 0;
    ID3D11ShaderResourceView* my_texture = NULL;

    static Module PreprocessingModules[3] = {
        Module::Module("FingerJetFXOSE", L"fjfxSample.exe", L"fjfx01.ist"),
        Module::Module("Test", L"fjfxSample.exe", L"fjfx01.ist"),
        Module::Module("Test2", L"fjfxSample.exe", L"fjfx01.ist"),
    };

    static int selectedExtractionModule = 0;
    static Module ExtractionModules[3] = {
        Module::Module("FingerJetFXOSE", L"fjfxSample.exe", L"fjfx01.ist"),
        Module::Module("Test", L"fjfxSample.exe", L"fjfx01.ist"),
        Module::Module("Test2", L"fjfxSample.exe", L"fjfx01.ist"),
    };

    std::wstring activeFile;

    void LoadInputImage(std::string imagePath)
    {
        bool ret = ImageVisualiser::LoadTextureFromFile(imagePath.c_str(), &my_texture, &my_image_width, &my_image_height);
        if (ret)
        {
            InputImage = imagePath;
        }
        std::cout << ret;
    }

    void Run()
    {
        activeFile = InputImage;
        for each (Module currentModule in PreprocessingModules)
        {
            if (!currentModule.Run(activeFile))
            {
                //TODO: visualise error
                break;
            }
            activeFile = currentModule.GetOutputFile();
        }

        ExtractionModules[selectedExtractionModule].Run(activeFile);
    }

    void RenderUI()
    {
        static char buf[64] = "Debug/Fingerprints/a001_03p.pgm";
        ImGui::Begin("FingerprintImage");
        ImGui::InputText("FilePath", buf, IM_ARRAYSIZE(buf));
        if (ImGui::Button("LoadImage"))
        {
            LoadInputImage(buf);
        }
        if (my_texture != NULL)
        {
            ImVec2 availSize = ImGui::GetContentRegionAvail();
            ImVec2 imageSize = ImageVisualiser::CalculateResolution(my_image_width, my_image_height, availSize.x, availSize.y);

            ImGui::Image((void*)my_texture, imageSize);
        }
        ImGui::End();

        RenderPreprocessing();

        RenderExtraction();

        RenderControl();
    }

    void RenderExtraction()
    {
        ImGui::Begin("Extraction");

        ImGui::Text("Select Module:");
        if (ImGui::BeginListBox("##", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(ExtractionModules); n++)
            {
                const bool is_selected = (selectedExtractionModule == n);
                if (ImGui::Selectable(ExtractionModules[n].GetModuleName().c_str(), is_selected))
                    selectedExtractionModule = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        ImGui::Text("Selected Module:");
        ImGui::CollapsingHeader(ExtractionModules[selectedExtractionModule].GetModuleName().c_str(), ImGuiTreeNodeFlags_Leaf);
        ExtractionModules[selectedExtractionModule].Render();
        ImGui::End();
    }
    void RenderPreprocessing()
    {
        ImGui::Begin("Preprocessing");
        // Simple reordering

        for (int n = 0; n < IM_ARRAYSIZE(PreprocessingModules); n++)
        {
            Module _module = PreprocessingModules[n];
            ImGui::PushID(n);
            bool showModuleDetails = ImGui::CollapsingHeader(_module.GetModuleName().c_str());
            

            // Our headers are both drag sources and drag targets
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers))
            {
                // Set payload to carry the index of our item (could be anything)
                ImGui::SetDragDropPayload("PREPROCESSING_MODULE", &n, sizeof(int));

                ImGui::EndDragDropSource();
            }
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREPROCESSING_MODULE"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(int));
                    int payload_n = *(const int*)payload->Data;

                    PreprocessingModules[n] = PreprocessingModules[payload_n];
                    PreprocessingModules[payload_n] = _module;
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::PopID();

            if (showModuleDetails)
            {
                _module.Render();
            }
        }
        ImGui::End();
    }
    void RenderControl()
    {
        ImGui::Begin("ControlPanel");
        if(ImGui::Button("Run"))
        {
            Run();
        }
        ImGui::End();
    }
};
