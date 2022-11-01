#pragma once
#include "Module.h"

class MatchingModule
{
public:
    virtual bool Run(std::string inputFilePath, std::string enrolledTemplatePath, std::string* out_outputFilePath);
    virtual void Render();
    double virtual GetResult();
protected:
    std::string ModuleName = "GenericModuleName";
    std::string OutputFileName;
    std::wstring ModuleExecutable;
};

