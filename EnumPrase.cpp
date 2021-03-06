#include "EnumPrase.h"
#include <iostream>

EnumPrase::EnumPrase(std::string path, std::string begin, std::string end)
{
    if(!path.empty())
        prase(path, begin, end);
}

void EnumPrase::setArrayNamePrefix(std::string prefix)
{
    array_prefix = prefix;
}
void EnumPrase::setArrayNameSuffix(std::string suffix)
{
    array_suffix = suffix;
}

bool EnumPrase::findNoteNoSpace(std::string stream, std::string note)
{
    bool ret = false;
    trimed(note, " /\r\n\t");
    std::string::size_type begin = stream.find("//");
    if (begin != std::string::npos)
    {
        stream.erase(0, begin);
        EnumPrase::trimed(stream, " /\r\n\t");
        if (!stream.compare(note))
        {
            ret = true;
        }
    }
    return ret;
}

bool EnumPrase::getROI(std::string path, std::string begin, std::string end)
{
    bool ret = true;
    std::ifstream file(path);
    EnumPos find_position = (begin.empty()) ? (ENUM_END) : (ENUM_BEGIN);

    if (!file.is_open())
    {
        throw std::invalid_argument("fatal error: " + path + " open filed!");
    }

    m_roi.clear();
    while (file.good())
    {
        std::string line;
        std::getline(file, line);
        switch (find_position)
        {
        case ENUM_BEGIN:
            if (findNoteNoSpace(line, std::string(begin)))
                find_position = ENUM_END;
            break;
        case ENUM_END:
            m_roi.append(line + '\n');
            if ((!end.empty()) && findNoteNoSpace(line, std::string(end)))
                find_position = ENUM_BEGIN;
            break;
        default:
            break;
        }
    }
    file.close();
    return ret;
}

void EnumPrase::frontTrimed(std::string &str, std::string delem)
{
    std::string::size_type pos = str.find_first_not_of(delem, 0);
    if (pos != std::string::npos)
    {
        str.erase(0, pos);
    }
}

void EnumPrase::trimed(std::string &str, std::string delem)
{
    std::string::size_type pos = str.find_first_of(delem, 0);
    while (pos != std::string::npos)
    {
        str.erase(pos, 1);
        pos = str.find_first_of(delem, 0);
    }
}

void EnumPrase::removeNote(std::string &stream)
{
    std::string::size_type end = std::string::npos;
    std::string::size_type begin = std::string::npos;

    if (stream.empty())
        return;
    do
    {
        begin = stream.find("/*");
        if (begin != std::string::npos)
        {
            end = stream.find("*/", begin);
            if (begin != std::string::npos)
                stream.erase(begin, end - begin + 2);
        }
        else
        {
            begin = stream.find("//");
            if (begin != std::string::npos)
            {
                end = stream.find("\n", begin);
                if (begin != std::string::npos)
                    stream.erase(begin, end - begin);
            }
        }
    } while (begin != std::string::npos);
}

std::list<std::string> EnumPrase::split(std::string stream, std::string seperator)
{
    std::string::size_type pos_comma;
    std::list<std::string> ret;

    if (!stream.empty() && !seperator.empty())
    {
        trimed(stream, " \r\n\t");
        do
        {
            pos_comma = stream.find(seperator);
            if (pos_comma != std::string::npos)
            {
                std::string item = stream.substr(0, pos_comma);
                ret.push_back(item);
                stream.erase(0, pos_comma + 1);
            }
            else if (!stream.empty())
                ret.push_back(stream);
        } while (pos_comma != std::string::npos);
    }

    return ret;
}

bool EnumPrase::praseROI(std::string stream)
{
    bool ret = !stream.empty();
    std::string::size_type pos_enum;
    std::string::size_type pos_left_brace;
    std::string::size_type pos_right_brace;
    std::string::size_type pos_semicolon;

    removeNote(stream);
    while (ret)
    {
        ret = ret && ((pos_enum = stream.find("enum")) != std::string::npos);
        ret = ret && ((pos_left_brace = stream.find("{")) != std::string::npos);
        ret = ret && ((pos_right_brace = stream.find("}")) != std::string::npos);
        ret = ret && ((pos_semicolon = stream.find(";", pos_right_brace)) != std::string::npos);

        if (ret && (pos_enum < pos_left_brace) && (pos_left_brace < pos_right_brace) && (pos_right_brace < pos_semicolon))
        {
            EnumInfo info;
            std::string member = stream.substr(pos_left_brace + sizeof('{'), pos_right_brace - pos_left_brace - sizeof('{'));
            info.name = stream.substr(pos_enum + sizeof("enum"), pos_left_brace - pos_enum - sizeof("enum"));
            info.typedef_name = stream.substr(pos_right_brace + sizeof('}'), pos_semicolon - pos_right_brace - sizeof('}'));
            trimed(info.name, " \r\n\t");
            trimed(info.typedef_name, " \r\n\t");
            if (!info.name.empty())
            {
                info.name = array_prefix + info.name + array_suffix;
            }
            else if(!info.typedef_name.empty())
            {
                info.name = array_prefix + info.typedef_name + array_suffix;
            }
            else
            {
                throw std::invalid_argument("fatal error: enum type name is empty!");
            }
            
            stream.erase(0, pos_semicolon + 1);
            info.member = split(member, ",");
            m_result.push_back(info);
        }
    }

    return true;
}

bool EnumPrase::prase(std::string path, std::string begin, std::string end)
{
    bool ret = false;

    m_result.clear();
    ret = getROI(path, begin, end) && praseROI(m_roi);

    return ret;
}

bool EnumPrase::prase(std::vector<std::string> path, std::string begin, std::string end)
{
    bool ret = true;

    m_result.clear();
    for (std::vector<std::string>::iterator i = path.begin(); i != path.end() && ret; i++)
    {
        ret = getROI(*i, begin, end) && praseROI(m_roi);
    }

    return ret;
}

std::list<EnumPrase::EnumInfo> EnumPrase::result()
{
    return m_result;
}

EnumPrase::~EnumPrase()
{
}
