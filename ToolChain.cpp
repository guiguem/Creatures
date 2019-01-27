#include "ToolChain.h"

std::unique_ptr<Tool> ToolChain::CreateTool(std::string toolName, std::string noise)
{
    return Tool::make(toolName, noise);
};