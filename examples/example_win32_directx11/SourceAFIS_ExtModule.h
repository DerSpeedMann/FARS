#pragma once
#include "Module.h"

class SourceAFIS_ExtModule : public Module
{
public:
    SourceAFIS_ExtModule();
    void Render() override;

    bool Run(std::string inputFile, std::string* out_outputFile) override;
};

