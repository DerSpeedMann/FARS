#pragma once
#include "Module.h"

class SourceAFIS_TemplateConverter : public Module
{
    SourceAFIS_TemplateConverter();
    void Render() override;

    bool Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath) override;
};

