#pragma once
#include <string>

class Module
{
public:
    Module();
    ~Module();
    virtual void Render();
    virtual bool Run(std::wstring inputFile);
    std::wstring GetOutputFile();
    std::string GetModuleName();

protected:
    std::string ModuleName = "GenericModuleName";
    std::wstring OutputFile;
    std::wstring ModuleExecutable;

    void SetOutputFile(std::wstring fileName);
};

