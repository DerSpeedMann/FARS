#include "ErrorHandler.h"
#include "imgui.h"
#include <iostream>
#include <sstream> 


ErrorHandler::ErrorHandler(ModuleType activeModuleType)
{
    ActiveModuleType = activeModuleType;
    ErrorCode = 0;
    ErrorMessage = "";
}
void ErrorHandler::setModuleError(int errorCode, std::string errorMessage, std::string moduleName)
{
    ErrorCode = errorCode;
    ErrorMessage = errorMessage;
    ModuleName = moduleName;
    ErrorActive = true;
}
void ErrorHandler::setNonModuleError(std::string errorMessage)
{
    ActiveModuleType = ModuleType::None;
    ErrorMessage = errorMessage;
    ErrorActive = true;
}

const char* ErrorHandler::getErrorString()
{
    std::ostringstream errorText;
    if (ActiveModuleType != ModuleType::None)
    {
        errorText << "Error in " << ActiveModuleType << " " << ModuleName << " " << ErrorCode << ": " << ErrorMessage;
        return errorText.str().c_str();
    }

    return ErrorMessage.c_str();
}

