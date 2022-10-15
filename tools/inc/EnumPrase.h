#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

class EnumPrase
{
public:
    struct EnumInfo
    {
        std::string name;
        std::string typedef_name;
        std::list<std::string> member;
    };

protected:
    enum EnumPos
    {
        ENUM_BEGIN,
        ENUM_END
    };

    std::string m_roi;
    std::string array_prefix;
    std::string array_suffix;
    std::list<EnumPrase::EnumInfo> m_result;

    std::list<std::string> split(std::string stream, std::string seperator);
    bool findNoteNoSpace(std::string stream, std::string note);
    bool getROI(std::string path, std::string begin, std::string end);
    virtual bool praseROI(std::string stream);

public:
    static void frontTrimed(std::string &str, std::string delem);
    static void trimed(std::string &str, std::string delem);
    static void removeNote(std::string &str);

    EnumPrase(std::string path = std::string(), std::string begin = std::string(), std::string end = std::string());
    bool prase(std::string path, std::string begin = std::string(), std::string end = std::string());
    bool prase(std::vector<std::string> path, std::string begin = std::string(), std::string end = std::string());
    void setArrayNamePrefix(std::string prefix);
    void setArrayNameSuffix(std::string suffix);
    std::list<EnumPrase::EnumInfo> result();
    virtual ~EnumPrase();
};