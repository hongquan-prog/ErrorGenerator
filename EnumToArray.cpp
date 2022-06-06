#include "EnumToArray.h"

EnumToArray::EnumToArray(const std::list<EnumPrase::EnumInfo>& info)
{
    convert(info);
}

bool EnumToArray::convert(const std::list<EnumPrase::EnumInfo>& info)
{
    bool ret = info.size();

    m_output.clear();
    m_output.str("");
    std::list<EnumPrase::EnumInfo>::const_iterator p = info.begin();

    for (; ret && p != info.end(); p++)
    {
        m_output << "static const char* ";

        if((*p).name.empty())
            m_output << (*p).typedef_name;
        else    
            m_output << (*p).name;

        m_output << "[] = {";
        std::list<std::string>::const_iterator q = (*p).member.begin();

        for(; q != (*p).member.end(); q++)
        {
            m_output << "\n\t\"";
            std::string::size_type pos = (*q).find("=");
            if(pos != std::string::npos)
                m_output << (*q).substr(0, pos) << "\",";
            else
                m_output << *q << "\",";
        }
        m_output << "\n};\n\n";
    }
    return ret;
}

std::string EnumToArray::result()
{
    return m_output.str();
}

EnumToArray::~EnumToArray()
{
}