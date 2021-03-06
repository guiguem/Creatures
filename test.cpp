#include <iostream>
#include <string>

#include "ToolChain.h"

int main()
{
    std::cout << "main" << std::endl;
    ToolChain toolchain;
    auto wrench = toolchain.CreateTool("Wrench", "splash");
    auto wrench2 = toolchain.CreateTool("Wrench", "bing");
    auto screwDriver = toolchain.CreateTool("ScrewDriver", "paff");
    wrench->makeNoise();
    wrench2->makeNoise();
    screwDriver->makeNoise();
    return 0;
}
