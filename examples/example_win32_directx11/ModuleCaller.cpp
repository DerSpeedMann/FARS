#include "ModuleCaller.h"
#include "Module.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <codecvt>


namespace ModuleCaller
{
    static const std::wstring ModulePath = L"\\Modules\\";
    static const std::wstring FingerprintPath = L"\\Fingerprints\\";
    static const std::wstring TempPath = L"\\tmp\\";
    static std::wstring ExePath;

    std::wstring GetExePath() {
        if (ExePath.empty())
        {
            TCHAR buffer[MAX_PATH + 1] = L"";
            GetModuleFileName(NULL, buffer, MAX_PATH);
            std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
            ExePath = std::wstring(buffer).substr(0, pos);
        }
        return ExePath;
    }
    std::wstring GetTmpPath()
    {
        return GetExePath() + TempPath;
    }
    std::wstring GetFingerprintsPath()
    {
        return GetExePath() + FingerprintPath;
    }
    std::wstring GetModulePath()
    {
        return GetExePath() + ModulePath;
    }

    std::string stringConvert(std::wstring wstring) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(wstring);
    }
    std::wstring stringConvert(std::string string) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(string);
    }

    int CallModule(std::wstring executablePath, std::wstring parameters)
    {
        // additional information
        STARTUPINFO si{ sizeof(si) };
        PROCESS_INFORMATION pi;

        DWORD exitCode;
        std::wstring fullParameters = executablePath + L" " + parameters;

        // start the program up
        if ( !CreateProcess(executablePath.c_str(),   // the path
            LPWSTR(fullParameters.c_str()),       // Parameters
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si,            // Pointer to STARTUPINFO structure
            &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
        ))
        {
            printf("CreateProcess failed (%d).\n", GetLastError());
            return -1;
        }

        std::wcout << L"started programm " + executablePath + L"\n";
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exitCode);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return exitCode;
    }
};
