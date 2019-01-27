#ifndef TOOL_H
#define TOOL_H
#include "factory.h"

struct Tool : Factory<Tool, std::string>
{
    Tool(Key) {}
    virtual void makeNoise() = 0;
};
#endif