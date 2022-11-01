#pragma once
#include <string>

class Module
{
public:
    Module();
    ~Module();
    virtual void Render();
    virtual bool Run(std::string inputFilePath, bool enrollMode, std::string* out_outputFilePath);
    std::string GetModuleName();

protected:
    std::string ModuleName = "GenericModuleName";
    std::string OutputFileNames[2];
    std::string ModuleExecutable;
};

