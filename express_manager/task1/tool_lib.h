#pragma once
#include <string>
#include <unistd.h>

std::string cwd();
void parse(const std::string cmd_line, std::string parsed[]);
void panic(std::string info);
bool is_file_exist(const char *filename);
bool is_account_exist(const char *account_user_name);
bool is_express_exist(const char *express_ID);
void bad_cin_handler();
const std::string current_time();
void add_balance_to(std::string admin_name, int amount);
