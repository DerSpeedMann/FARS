#pragma once
#include <string>

namespace ModuleCaller
{
    std::wstring GetExePath();
    std::wstring GetTmpPath();
    std::wstring GetFingerprintsPath();
    std::wstring GetModulePath();
    int CallModule(std::wstring executablePath, std::wstring parameters);
};

