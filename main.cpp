
#include "CmdParamPraser.h"
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
            // 删除最后一个换行符号，否则每运行一次增加一行
            file << ret.str().substr(0, ret.str().length() - 1);
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
    std::cout << "    -input-begin   Start position of enum" << std::endl;
    std::cout << "    -input-end     End position of enum " << std::endl;
    std::cout << "    -output-begin  Start position of output" << std::endl;
    std::cout << "    -output-end    End position of output " << std::endl;
    std::cout << "    -i             Input file path of enum " << std::endl;
    std::cout << "    -o             Output file path of error code " << std::endl;
    std::cout << "example ./error-generator \"test.h\" \"test1.h\" -o=\"exp.c\"" << std::endl;
    std::cout << "example ./error-generator \"test.h\" -input-begin=\"//error code begin\" -input-end=\"//error code end\"" << std::endl;
    std::cout << "example ./error-generator \"test.h\" -output-begin=\"// generate begin\" -output-end=\"// generate end\"" << std::endl;
    std::cout << "example ./error-generator -i=\"test.h\" -o=\"test.c\" -input-begin=\"//error code begin\" -input-end=\"//error code end\" -output-begin=\"// generate begin\" -output-end=\"// generate end\"" << std::endl;
}

int main(int argc, char *argv[])
{
    CmdParamPraser cli(argc - 1, argv + 1);

    // 指定输入文件
    std::vector<std::string> input_file = cli.readArguments("-i");
    // 指定输出文件
    std::string output_file = cli.readArgument("-o");
    // 指定文件中枚举量的开始位置
    std::string input_begin = cli.readArgument("-input-begin");
    // 指定文件中枚举量的结束位置
    std::string input_end = cli.readArgument("-input-end");
    // 指定文件输出代码的开始位置
    std::string output_begin = cli.readArgument("-output-begin");
    // 指定文件输出代码的结束位置
    std::string output_end = cli.readArgument("-output-end");
    // 枚举量结果
    std::list<EnumPrase::EnumInfo> enum_info;

    if(!std::string("-h").compare(argv[1]))
    {
        manual();
        return 0;
    }

    if (!input_file.size())
    {
        std::cout << "fatal error: no input files" << std::endl;
        return -1;
    }

    for (std::vector<std::string>::iterator i = input_file.begin(); i != input_file.end(); i++)
    {
        try
        {
            EnumPrase prase(*i, input_begin, input_end);
            std::list<EnumPrase::EnumInfo> result = prase.result();
            for (std::list<EnumPrase::EnumInfo>::iterator j = result.begin(); j != result.end(); j++)
            {
                (*j).name = std::string("s_") + (*j).typedef_name + std::string("_err_str");
                (*j).typedef_name.clear();
                enum_info.push_back(*j);
            }
        }
        catch (std::invalid_argument& e)
        {
            std::cout << e.what() << std::endl;
            return -1;
        }
    }

    EnumToArray error_str(enum_info);
    saveResult(output_file, output_begin, output_end, generateStructure("s_error_str_array", std::to_string(enum_info.size())) + \
    error_str.result() + gernerateInitializeFunction("error_str_init", "s_error_str_array", enum_info));
    std::cout << "generate success!" << std::endl;
    return 0;
}