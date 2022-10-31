#pragma once
#include "Module.h"

class MatchingModule : public Module
{
public:
    void SetTemplateFile(std::string fileName);
protected:
    std::string TemplateFile;
};

