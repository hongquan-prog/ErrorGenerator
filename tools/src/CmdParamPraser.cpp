#include "CmdParamPraser.h"

std::vector<std::string> CmdParamPraser::readArguments(std::string option)
{
    std::vector<std::string> ret;
    std::vector<std::string>::iterator i = parameter.begin();

    for (; i != parameter.end(); i++)
    {
        std::string::size_type index = (*i).find("=");
        std::string prefix = (index != std::string::npos) ? ((*i).substr(0, index)) : ((*i));
        std::string suffix = (index != std::string::npos) ? ((*i).substr(index + 1)) : ("");

        if (!option.compare(prefix))
        {
            ret.push_back(suffix);
        }
    }

    return ret;
}

std::string CmdParamPraser::readArgument(std::string option)
{
    std::string ret;
    std::vector<std::string>::iterator i = parameter.begin();

    for (; i != parameter.end(); i++)
    {
        std::string::size_type index = (*i).find("=");
        std::string prefix = (index != std::string::npos) ? ((*i).substr(0, index)) : ((*i));
        std::string suffix = (index != std::string::npos) ? ((*i).substr(index + 1)) : ("");

        if (!option.compare(prefix))
        {
            ret = suffix;
            break;
        }
    }

    return ret;
}

CmdParamPraser::CmdParamPraser(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        parameter.push_back(argv[i]);
    }
}

CmdParamPraser::~CmdParamPraser(void)
{
}