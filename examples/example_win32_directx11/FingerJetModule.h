#pragma once
#include "Module.h"

class FingerJetModule : public Module
{
public:
    FingerJetModule();
    void Render() override;

    bool Run(std::string inputFile, bool enrollMode, std::string* out_outputFile, ErrorHandler* errorHandler) override;
};
