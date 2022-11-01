#pragma once
#include <string>

namespace ModuleCaller
{
    std::string GetExePath();
    std::string GetTmpPath();
    std::string GetFingerprintsPath();
    std::string GetModulePath();
    std::string stringConvert(std::wstring wstring);
    std::wstring stringConvert(std::string string);
    int CallModule(std::string executablePath, std::string parameters);
};

