#pragma once
#include "ModuleType.h"
#include <string>

class ErrorHandler
{
public:
    bool ErrorActive = false;

    ModuleType ActiveModuleType;
    std::string ModuleName;

    int ErrorCode;
    std::string ErrorMessage;

    ErrorHandler(ModuleType activeModuleType);
    void setModuleError(int errorCode, std::string errorMessage, std::string moduleName);
    void setNonModuleError(std::string errorMessage);
    const char* getErrorString();
};


