#include "FarsUI.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <iostream>
#include <d3d11.h>

//Helpers
#include "ModuleCaller.h"
#include "ImageVisualizer.h"

//Modules
#include "Module.h"
#include "MatchingModule.h"
#include "ImageConvertingModule.h"
#include "FingerJetModule.h"
#include "SourceAFIS_ExtModule.h"
#include "SourceAFIS_MatModule.h"
#include "ModuleType.h"

namespace FarsUI
{
    // General
    std::string templateFile;

    std::string activeFile;
    std::string matchingScore = "0";
    bool enrollView = true;

    ErrorHandler errorHandler = ErrorHandler(ModuleType::None);

    bool renderPoIs = false;

    char* viewModeText[2] = { "Matching View", "Enroll View" };
    char* runBText[2] = { "Start Matching", "Enroll Fingerprint" };
    char* changeViewBText[2] = { "Switch to Enroll", "Switch to Matching" };
    char* testRenderBText[2] = { "Test PoI`s", "Disable PoI`s" };

    ImGuiInputTextFlags_ fileLoadInputFlags[2] = {ImGuiInputTextFlags_ReadOnly, ImGuiInputTextFlags_None };

    static char enrollInputBuffer[128] = "Debug/Fingerprints/a001_03p.pgm";
    static char matchingInputBuffer[128] = "Debug/Fingerprints/a001_03p.pgm";
    
    static std::unique_ptr<Module> PreprocessingModules[1] = {
        std::make_unique<ImageConvertingModule>(),
    };
    static std::unique_ptr<Module> ExtractionModules[2] = {
        std::make_unique<FingerJetModule>(),
        std::make_unique<SourceAFIS_ExtModule>(),
    };
    static std::unique_ptr<MatchingModule> MatchingModules[1] = {
        std::make_unique<SourceAFIS_MatModule>(),
    };

    // Enroll View
    std::string enrollInputImage;
    int enroll_image_width = 0;
    int enroll_image_height = 0;
    ID3D11ShaderResourceView* enroll_texture = NULL;

    static bool enrollPreprocessingSelection[IM_ARRAYSIZE(PreprocessingModules)] = {};
    static int enrollSelectedExtractionModule = 0;

    // Matching View
    std::string matchingInputImage;
    int matching_image_width = 0;
    int matching_image_height = 0;
    ID3D11ShaderResourceView* matching_texture = NULL;

    static bool matchingPreprocessingSelection[IM_ARRAYSIZE(PreprocessingModules)] = {};  
    static int matchingSelectedExtractionModule = 0;
    static int matchingSelectedMatchingModule = 0;


    bool LoadInputImage(std::string imagePath, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
    {
        if (!ImageVisualizer::LoadTextureFromFile(imagePath.c_str(), out_srv, out_width, out_height))
        {
            std::cout << printf("Loading Image failed!\n");
            return false;
        }
        return true;
    }

    void Run()
    {
        std::string out = "";
        matchingScore = "0";
        bool *preprocessingSelection;
        int selectedExtractionModule;

        if (enrollInputImage.empty())
        {
            std::string error = "No enroll Fingerprint selected!\n";
            std::cout << error;
            errorHandler.setNonModuleError(error);
            return;
        }

        if (enrollView) {
            templateFile = "";
            activeFile = enrollInputImage;
            preprocessingSelection = enrollPreprocessingSelection;
            selectedExtractionModule = enrollSelectedExtractionModule;
        }
        else
        {
            if (templateFile.empty())
            {
                std::string error = "No Fingerprint enrolled!\n";
                std::cout << error;
                return;
            }
            if (matchingInputImage.empty())
            {
                std::cout << "No matching Fingerprint selected!\n";
                return;
            }
            activeFile = matchingInputImage;
            preprocessingSelection = matchingPreprocessingSelection;
            selectedExtractionModule = matchingSelectedExtractionModule;

        }
        
        for (int i = 0; i < IM_ARRAYSIZE(PreprocessingModules); i++)
        {
            if (!preprocessingSelection[i])
            {
                continue;
            }

            if (!(*PreprocessingModules[i]).Run(activeFile, enrollView, &out))
            {
                std::cout << "Running Preprocessing Module Failed!\n";
                return;
            }
            activeFile = out;
        }

        if ((*ExtractionModules[selectedExtractionModule]).Run(activeFile, enrollView, &out))
        {
            std::cout << "Running Extraction Module Failed!\n";
            return;
        }
        activeFile = out;

        if (enrollView) {
            templateFile = out;
            std::cout << "Enrolled Fingerprint!\n";
            return;
        }

        if ((*MatchingModules[matchingSelectedMatchingModule]).Run(activeFile, templateFile, &out))
        {
            std::cout << "Running Matching Module Failed!\n";
            return;
        }

        double score = (*MatchingModules[matchingSelectedMatchingModule]).GetResult();
        matchingScore = std::to_string(score).c_str();
    }

    void RenderUI()
    {
        bool* preprocessingSelectionArray;
        int* extractionSelection;
        if (enrollView)
        {
            preprocessingSelectionArray = enrollPreprocessingSelection;
            extractionSelection = &enrollSelectedExtractionModule;
        }
        else
        {
            preprocessingSelectionArray = matchingPreprocessingSelection;
            extractionSelection = &matchingSelectedExtractionModule;
        }

        RenderInput();

        RenderPreprocessing(preprocessingSelectionArray);

        RenderExtraction(extractionSelection);

        if (!enrollView)
        {
            RenderMatching();
        }

        RenderControl();

        RenderError(&errorHandler);
    }

    void RenderInput() {

        ImGui::Begin("Fingerprint Images");

        if (ImGui::CollapsingHeader("Enrolled Image", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (enrollView) {
                ImGui::InputText("File Path", enrollInputBuffer, IM_ARRAYSIZE(enrollInputBuffer), fileLoadInputFlags[enrollView]);
                if (ImGui::Button("Load Image") && LoadInputImage(enrollInputBuffer, &enroll_texture, &enroll_image_width, &enroll_image_height))
                {
                    enrollInputImage = enrollInputBuffer;
                }
            }
            if (enroll_texture != NULL)
            {
                
                ImVec2 availSize = ImGui::GetContentRegionAvail();
                ImVec2 imageSize = ImageVisualizer::CalculateResolution(enroll_image_width, enroll_image_height, availSize.x, availSize.y / 2);

                ImGui::Image((void*)enroll_texture, imageSize);

                if (renderPoIs)
                {
                    ImVec2 winPos = ImGui::GetWindowPos();
                    ImageVisualizer::RenderPoI(ImVec2(winPos.x + 100, winPos.y + 150), 0xFF0000FF);
                }
            }
        }
        

        if (enrollView) {
            ImGui::End();
            return;
        }

        if (ImGui::CollapsingHeader("Matching Image", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::InputText("Matching File Path", matchingInputBuffer, IM_ARRAYSIZE(matchingInputBuffer), fileLoadInputFlags[!enrollView]);
            if (ImGui::Button("Load Matching Image") && LoadInputImage(matchingInputBuffer, &matching_texture, &matching_image_width, &matching_image_height))
            {
                matchingInputImage = matchingInputBuffer;
            }
            if (matching_texture != NULL)
            {
                ImVec2 availSize = ImGui::GetContentRegionAvail();
                ImVec2 imageSize = ImageVisualizer::CalculateResolution(matching_image_width, matching_image_height, availSize.x, availSize.y);

                ImGui::Image((void*)matching_texture, imageSize);
            }
        }

        ImGui::End();
    }
    void RenderPreprocessing(bool *preprocessingSelectionArray)
    {
        ImGui::Begin("Preprocessing");

        ImGui::Text("Select Module:");
        if (ImGui::BeginListBox("##", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(PreprocessingModules); n++)
            {
                if (ImGui::Selectable((*PreprocessingModules[n]).GetModuleName().c_str(), preprocessingSelectionArray[n]))
                {
                    preprocessingSelectionArray[n] ^= 1;
                }
            }
            ImGui::EndListBox();
        }

        ImGui::Text("Selected Modules:");
        // Simple reordering
        for (int n = 0; n < IM_ARRAYSIZE(PreprocessingModules); n++)
        {
            if (!preprocessingSelectionArray[n])
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
    void RenderExtraction(int *extractionSelection)
    {
        ImGui::Begin("Extraction");

        ImGui::Text("Select Module:");
        if (ImGui::BeginListBox("##", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(ExtractionModules); n++)
            {
                const bool is_selected = (*extractionSelection == n);
                if (ImGui::Selectable((*ExtractionModules[n]).GetModuleName().c_str(), is_selected))
                    *extractionSelection = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        ImGui::Text("Selected Module:");

        ImGui::CollapsingHeader((*ExtractionModules[*extractionSelection]).GetModuleName().c_str(), ImGuiTreeNodeFlags_Leaf);
        (*ExtractionModules[*extractionSelection]).Render();
        ImGui::End();
    }
    void RenderMatching() {
        ImGui::Begin("Matching");

        ImGui::Text("Select Module:");
        if (ImGui::BeginListBox("##", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(MatchingModules); n++)
            {
                const bool is_selected = (matchingSelectedMatchingModule == n);
                if (ImGui::Selectable((*MatchingModules[n]).GetModuleName().c_str(), is_selected))
                    matchingSelectedMatchingModule = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        ImGui::Text("Selected Module:");

        ImGui::CollapsingHeader((*MatchingModules[matchingSelectedMatchingModule]).GetModuleName().c_str(), ImGuiTreeNodeFlags_Leaf);
        (*MatchingModules[matchingSelectedMatchingModule]).Render();
        ImGui::End();
    }
    void RenderControl()
    {
        ImGui::Begin("Control Panel");
        ImGui::Text(viewModeText[enrollView]);

        if (ImGui::Button(runBText[enrollView]))
        {
            Run();
        }

        if (!templateFile.empty())
        {
            if (ImGui::Button(changeViewBText[enrollView]))
            {
                enrollView ^= true;
            }

            std::ostringstream scoreText;
            scoreText << "Match Score: " << matchingScore.c_str();
            ImGui::Text(scoreText.str().c_str());
        }


        if (ImGui::Button(testRenderBText[renderPoIs]))
        {
            renderPoIs ^= true;
        }
        ImGui::End();
    }
    void RenderError(ErrorHandler *errorHandler)
    {
        if ((*errorHandler).ErrorActive)
        {
            (*errorHandler).ErrorActive = false;
            ImGui::OpenPopup("Error");
        }
            

        bool open = true;
        if (ImGui::BeginPopupModal("Error", &open))
        {
            ImGui::Text((*errorHandler).getErrorString());
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
};
