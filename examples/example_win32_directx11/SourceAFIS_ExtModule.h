#pragma once
#include "Module.h"

class SourceAFIS_ExtModule : public Module
{
public:
    SourceAFIS_ExtModule();
    void Render() override;

    bool Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath) override;
};

