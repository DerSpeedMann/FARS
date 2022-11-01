#include "ModuleCaller.h"
#include "Module.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <codecvt>
#include <stdio.h>
#include <psapi.h>


namespace ModuleCaller
{
    static const std::string ModulePath = "\\Modules\\";
    static const std::string FingerprintPath = "\\Fingerprints\\";
    static const std::string TempPath = "\\tmp\\";
    static std::string ExePath;

    std::string GetExePath() {
        if (ExePath.empty())
        {
            TCHAR buffer[MAX_PATH + 1] = L"";
            GetModuleFileName(NULL, buffer, MAX_PATH);
            std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
            ExePath = stringConvert(std::wstring(buffer).substr(0, pos));
        }
        return ExePath;
    }
    std::string GetTmpPath()
    {
        return GetExePath() + TempPath;
    }
    std::string GetFingerprintsPath()
    {
        return GetExePath() + FingerprintPath;
    }
    std::string GetModulePath()
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

    int CallModule(std::string executablePath, std::string parameters)
    {
        // additional information
        STARTUPINFO si{ sizeof(si) };
        PROCESS_INFORMATION pi;

        DWORD exitCode;
        std::wstring fullParameters = stringConvert(executablePath + " " + parameters);

        // start the program up
        if ( !CreateProcess(stringConvert(executablePath).c_str(),   // the path
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

        std::string base_filename = executablePath.substr(executablePath.find_last_of("/\\") + 1);

        std::cout << "started programm " + base_filename + "\n";
        
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exitCode);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return exitCode;
    }
};
