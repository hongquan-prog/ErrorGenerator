#pragma once

#include "EnumPrase.h"
#include <sstream>

class EnumToArray
{
private:
    std::ostringstream m_output;

public: 
    EnumToArray(const std::list<EnumPrase::EnumInfo>& info);
    bool convert(const std::list<EnumPrase::EnumInfo>& info);
    std::string result();
    virtual ~EnumToArray();
};


