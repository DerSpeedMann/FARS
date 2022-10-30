#pragma once
#include "Module.h"

class FingerJetModule : public Module
{
public:
    FingerJetModule();
    void Render() override;

    bool Run(std::wstring inputFile) override;
};
