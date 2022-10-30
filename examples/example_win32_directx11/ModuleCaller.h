#pragma once
#include <string>

namespace ModuleCaller
{
    std::wstring GetExePath();
    std::wstring GetTmpPath();
    std::wstring GetFingerprintsPath();
    std::wstring GetModulePath();
    std::wstring stringConvert(std::string string);
    std::string stringConvert(std::wstring string);
    int CallModule(std::wstring executablePath, std::wstring parameters);
};

