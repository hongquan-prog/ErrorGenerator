#pragma once

#include <vector>
#include <string>
#include <iostream>

class CmdParamPraser
{
protected:
    std::vector<std::string> parameter;

public:
    CmdParamPraser(int argc, char **argv);
    std::string readArgument(std::string option);
    std::vector<std::string> readArguments(std::string option);
    ~CmdParamPraser();
};
