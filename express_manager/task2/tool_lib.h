#pragma once
#include <unistd.h>
#include <string>

struct ToolLib {
    std::string cwd();
    void parse(const std::string cmd_line, std::string parsed[]);
    void panic(std::string info);
    bool is_file_exist(const char *filename);
    bool is_account_exist(const char *account_user_name);
    bool is_express_exist(const char *express_ID);
    void rm_express(const char* express_ID);
    void bad_cin_handler();
    const std::string current_time();
    int account_type(std::string nickname);
    int exp_type(std::string express_ID);
};
