#pragma once
#include "Module.h"

class MatchingModule : public Module
{
public:
    void SetTemplateFile(std::string fileName);
    double virtual GetResult();
protected:
    std::string TemplateFile;
};

