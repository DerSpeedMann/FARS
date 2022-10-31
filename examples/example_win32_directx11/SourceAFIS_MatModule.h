#pragma once
#include "Module.h"

class SourceAFIS_MatModule : public Module
{
public:
    SourceAFIS_MatModule();
    void Render() override;

    bool Run(std::string inputFile, std::string* out_outputFile) override;
};

