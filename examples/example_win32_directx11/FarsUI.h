#pragma once
#include "ErrorHandler.h"

namespace FarsUI
{
    void RenderUI();
    void RenderInput();
    void RenderPreprocessing(bool* preprocessingSelectionArray);
    void RenderExtraction(int* extractionSelection);
    void RenderMatching();
    void RenderControl();
    void RenderError(ErrorHandler *handler);
};

