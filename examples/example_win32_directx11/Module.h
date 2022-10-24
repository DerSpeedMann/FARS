#pragma once
#include <string>

class Module
{
private:
    std::wstring OutputFile;
    std::string ModuleName;
    void SetOutputFile(std::wstring fileName);

public:
    std::wstring ModuleExecutable;
    Module(std::string moduleName, std::wstring moduleExecutable, std::wstring outputFileName);
    ~Module();
    void Render();
    bool Run(std::wstring inputFile);
    std::wstring GetOutputFile();
    std::string GetModuleName();
};

