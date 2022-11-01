#pragma once
#include "MatchingModule.h"

class SourceAFIS_MatModule : public MatchingModule
{
public:
    SourceAFIS_MatModule();
    void Render() override;
    bool Run(std::string inputFilePath, std::string enrolledTemplatePath, std::string* out_outputFilePath) override;
    double virtual GetResult() override;
};

