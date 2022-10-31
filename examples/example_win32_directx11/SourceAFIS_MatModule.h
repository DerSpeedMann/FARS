#pragma once
#include "MatchingModule.h"

class SourceAFIS_MatModule : public MatchingModule
{
public:
    SourceAFIS_MatModule();
    void Render() override;

    bool Run(std::string inputFile, std::string* out_outputFile) override;
};

