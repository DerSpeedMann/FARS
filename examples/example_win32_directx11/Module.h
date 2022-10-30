#pragma once
#include <string>

class Module
{
public:
    Module();
    ~Module();
    virtual void Render();
    virtual bool Run(std::string inputFile, std::string* out_outputFile);
    std::string GetOutputFile();
    std::string GetModuleName();

protected:
    std::string ModuleName = "GenericModuleName";
    std::string OutputFile;
    std::wstring ModuleExecutable;

    void SetOutputFile(std::string fileName);
};

