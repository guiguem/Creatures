#ifndef TOOLCHAIN_H
#define TOOLCHAIN_H
#include "Tool.h"

class ToolChain
{
    public:
    ToolChain(){}
    std::unique_ptr<Tool> CreateTool(std::string toolName, std::string noise);
};

#endif