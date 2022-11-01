#pragma once
#include "Module.h"

class ImageConvertingModule : public Module
{
public:
    ImageConvertingModule();

    void Render() override;

    bool Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath) override;
};
