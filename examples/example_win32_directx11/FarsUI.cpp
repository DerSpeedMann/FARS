#include "FarsUI.h"
#include "imgui.h"
#include "ModuleCaller.h"
#include "Module.h"
#include "FingerJetModule.h"
#include "ImageConvertingModule.h"
#include "ImageVisualiser.h"
#include "imgui_impl_dx11.h"
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <iostream>
#include <d3d11.h>

namespace FarsUI
{

    std::string InputImage;

    int template_image_width = 0;
    int template_image_height = 0;
    ID3D11ShaderResourceView* template_texture = NULL;

    
    static std::unique_ptr<Module> PreprocessingModules[3] = {
        std::make_unique<ImageConvertingModule>(),
        std::make_unique<Module>(),
        std::make_unique<Module>(),
    };
    static bool preprocessingSelection[IM_ARRAYSIZE(PreprocessingModules)] = {};

    static std::unique_ptr<Module> ExtractionModules[3] = {
        std::make_unique<FingerJetModule>(),
        std::make_unique<Module>(),
        std::make_unique<Module>(),
    };
    static int selectedExtractionModule = 0;
    
    std::string activeFile;

    void LoadInputImage(std::string imagePath)
    {
        if (!ImageVisualiser::LoadTextureFromFile(imagePath.c_str(), &template_texture, &template_image_width, &template_image_height))
        {
            std::cout << printf("Loading Image failed!\n");
            return;
        }
        
        InputImage = imagePath;
    }

    void Run()
    {
        if (InputImage.empty())
        {
            std::cout << "No Fingerprint selected!\n";
            return;
        }

        std::string out = "";
        activeFile = InputImage;
        for (int i = 0; i < IM_ARRAYSIZE(PreprocessingModules); i++)
        {
            if (!preprocessingSelection[i])
            {
                continue;
            }

            if (!(*PreprocessingModules[i]).Run(activeFile, &out))
            {
                //TODO: visualise error
                continue;
            }
            activeFile = out;
        }

        (*ExtractionModules[selectedExtractionModule]).Run(activeFile, &out);
    }

    void RenderUI()
    {
        static char buf[128] = "Debug/Fingerprints/a001_03p.pgm";
        ImGui::Begin("FingerprintImage");
        ImGui::InputText("FilePath", buf, IM_ARRAYSIZE(buf));
        if (ImGui::Button("LoadImage"))
        {
            LoadInputImage(buf);
        }
        if (template_texture != NULL)
        {
            ImVec2 availSize = ImGui::GetContentRegionAvail();
            ImVec2 imageSize = ImageVisualiser::CalculateResolution(template_image_width, template_image_height, availSize.x, availSize.y);

            ImGui::Image((void*)template_texture, imageSize);
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
                if (ImGui::Selectable((*ExtractionModules[n]).GetModuleName().c_str(), is_selected))
                    selectedExtractionModule = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        ImGui::Text("Selected Module:");

        ImGui::CollapsingHeader((*ExtractionModules[selectedExtractionModule]).GetModuleName().c_str(), ImGuiTreeNodeFlags_Leaf);
        (*ExtractionModules[selectedExtractionModule]).Render();
        ImGui::End();
    }
    void RenderPreprocessing()
    {
        ImGui::Begin("Preprocessing");

        ImGui::Text("Select Module:");
        if (ImGui::BeginListBox("##", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(PreprocessingModules); n++)
            {
                if (ImGui::Selectable((*PreprocessingModules[n]).GetModuleName().c_str(), preprocessingSelection[n]))
                {
                    preprocessingSelection[n] ^= 1;
                }
            }
            ImGui::EndListBox();
        }

        ImGui::Text("Selected Modules:");
        // Simple reordering
        for (int n = 0; n < IM_ARRAYSIZE(PreprocessingModules); n++)
        {
            if (!preprocessingSelection[n])
            {
                continue;
            }

            ImGui::PushID(n);
            bool showModuleDetails = ImGui::CollapsingHeader((*PreprocessingModules[n]).GetModuleName().c_str());
            

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

                    PreprocessingModules[n].swap(PreprocessingModules[payload_n]);
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::PopID();

            if (showModuleDetails)
            {
                (*PreprocessingModules[n]).Render();
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
