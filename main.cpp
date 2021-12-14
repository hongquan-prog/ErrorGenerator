
#include "EnumPrase.h"
#include "EnumToArray.h"
#include <vector>

std::string gernerateInitializeFunction(std::string func_name,
                                        std::string array_name, const std::list<EnumPrase::EnumInfo> &info)
{
    std::ostringstream ret;

    ret << "void " << func_name << "()\n{";
    for (std::list<EnumPrase::EnumInfo>::const_iterator i = info.begin(); i != info.end(); i++)
    {
        std::list<std::string>::const_iterator j = (*i).member.begin();
        std::string::size_type begin = (*j).find('(') + 1;
        std::string::size_type end = (*j).find(')');
        std::string module_id = (*j).substr(begin, end - begin);
        ret << "\n\t" << array_name << "[" << module_id << "].exist = true;";
        ret << "\n\t" << array_name << "[" << module_id << "].last_error = " << (*i).member.size() - 1 << ";";
        ret << "\n\t" << array_name << "[" << module_id << "].error_array = ";
        ret << (*i).name;
        ret << ";\n";
    }
    ret << "}\n\n";

    ret << "const char* error_to_str(err_t errno)";
    ret << "\n{";
	ret << "\n\tstatic bool initialized = false;";
	ret << "\n\tunsigned short error_code = ERR_GET_ERROR_INDEX(errno);";
	ret << "\n\tunsigned short module_id = ERR_GET_MODULE_ID(errno);";
	ret << "\n\tif(!initialized)";
	ret << "\n\t{";
	ret << "\n\t\terror_str_init();";
	ret << "\n\t\tinitialized = true;";
	ret << "\n\t}";
	ret << "\n\tif(errno > 0)";
	ret << "\n\t\treturn \"Errno should less than 0\";";
	ret << "\n\tif(!s_error_str_array[module_id].exist)";
	ret << "\n\t\treturn \"Error code array isn't exist\";";
	ret << "\n\tif(s_error_str_array[module_id].last_error < error_code)";
	ret << "\n\t\treturn \"Error code out of range\";";
	ret << "\n\treturn s_error_str_array[module_id].error_array[error_code];";
    ret << "\n}";

    return ret.str();
}

std::string generateStructure(std::string array_name, std::string array_size)
{
    std::ostringstream ret;

    ret << "static struct error_str_t\n";
    ret << "{\n\t";
    ret << "bool exist;\n\t";
    ret << "int last_error;\n\t";
    ret << "const char ** error_array;\n";
    ret << "}" << array_name << "[" << array_size << "]";
    ret << ";\n\n";
    return ret.str();
}

void saveResult(std::string path, std::string output_begin, std::string output_end, std::string result)
{
    int step = 0;
    std::ostringstream ret;
    std::fstream file(path);
    if (file.is_open())
    {
        if(!output_begin.empty() && !output_end.empty())
        {
            std::string line;
            while(file.good())
            {
                std::getline(file, line);
                switch(step)
                {
                    case 0:
                        if(!line.compare(output_begin))
                        {
                            ret << line << std::endl;
                            ret << result << std::endl;
                            step = 1;
                            break;
                        }
                        ret << line << std::endl;
                        break;
                    case 1:
                        if(!line.compare(output_end))
                        {
                            ret << line << std::endl;
                            step = 2;
                        }
                        break;
                    default:
                        ret << line << std::endl; 
                        break; 
                }
            }
            file.close();
            file.open(path, std::ios::trunc | std::ios::out);
            file << ret.str();
        }
        else
            file << result;
        file.close();
    }
}

void manual()
{
    std::cout << "Usage: error-generator [options=] file..." << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "    -b\t Start position of enum" << std::endl;
    std::cout << "    -e\t End position of enum " << std::endl;
    std::cout << "    -i\t Input file path of enum " << std::endl;
    std::cout << "    -o\t Output file path of error code " << std::endl;
    std::cout << "example ./error-generator test.h test1.h -o=exp.c" << std::endl;
    std::cout << "example ./error-generator test.h -input-begin=\"//error code begin\" -input-end=\"//error code end\"" << std::endl;
    std::cout << "example ./error-generator test.h -output-begin=\"// generate begin\" -output-end=\"// generate end\"" << std::endl;
}

int main(int argc, char *argv[])
{
    std::vector<std::string> input_file;
    std::string output_file("output.c");
    // 指定文件中枚举量的开始位置
    std::string enum_begin;
    // 指定文件中枚举量的结束位置
    std::string enum_end;
    // 指定文件输出代码的开始位置
    std::string output_begin;
    // 指定文件输出代码的结束位置
    std::string output_end;
    // 枚举量结果
    std::list<EnumPrase::EnumInfo> enum_result;

    for (int i = 1; i < argc; i++)
    {
        std::string item = std::string(argv[i]);
        std::string::size_type index = item.find("=");
        std::string prefix = (index != std::string::npos) ? (item.substr(0, index)) : (item);
        std::string suffix = (index != std::string::npos) ? (item.substr(index + 1)) : ("");
        if (prefix.compare("-o") == 0)
            output_file = suffix;
        else if (prefix.compare("-i") == 0)
            input_file.push_back(suffix);
        else if (prefix.compare("-input-begin") == 0)
            enum_begin = suffix;
        else if (prefix.compare("-input-end") == 0)
            enum_end = suffix;
        else if (prefix.compare("-output-begin") == 0)
            output_begin = suffix;
        else if (prefix.compare("-output-end") == 0)
            output_end = suffix;
        else if (prefix.compare("-h") == 0)
        {
            manual();
            return 0;
        }
        else 
            input_file.push_back(argv[i]);
    }
    if (!input_file.size())
    {
        std::cout << "fatal error: no input files" << std::endl;
        return -1;
    }
    else
    {
        for (std::vector<std::string>::iterator i = input_file.begin(); i != input_file.end(); i++)
        {
            std::ifstream file(*i);
            if (!file.is_open())
            {
                std::cout << "fatal error: file open filed!" << std::endl;
                std::cout << "file path:" << *i << std::endl;
                return -2;
            }
        }
    }

    for (std::vector<std::string>::iterator i = input_file.begin(); i != input_file.end(); i++)
    {
        EnumPrase prase(*i, enum_begin, enum_end);
        std::list<EnumPrase::EnumInfo> result = prase.result();
        for (std::list<EnumPrase::EnumInfo>::iterator j = result.begin(); j != result.end(); j++)
        {
            (*j).name = std::string("s_") + (*j).typedef_name + std::string("_err_str");
            (*j).typedef_name.clear();
            enum_result.push_back(*j);
        }
    }

    EnumToArray error_str(enum_result);
    saveResult(output_file, output_begin, output_end, generateStructure("s_error_str_array", std::to_string(enum_result.size())) + \
    error_str.result() + gernerateInitializeFunction("error_str_init", "s_error_str_array", enum_result));
    std::cout << "generate success!" << std::endl;
    return 0;
}