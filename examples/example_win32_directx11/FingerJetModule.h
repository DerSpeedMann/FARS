#pragma once
#include "Module.h"

class FingerJetModule : public Module
{
public:
    FingerJetModule();
    void Render() override;

    bool Run(std::string inputFile, std::string* out_outputFile) override;
};
