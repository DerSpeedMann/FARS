#pragma once
#include "Module.h"

class ImageConvertingModule : public Module
{
public:
    ImageConvertingModule();

    void Render() override;

    bool Run(std::wstring inputFile);
};
