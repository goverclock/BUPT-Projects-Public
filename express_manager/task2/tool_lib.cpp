#include "tool_lib.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

#include "admin.h"
#include "defs.h"

extern ToolLib tl;

std::string ToolLib::cwd() {
    auto p = get_current_dir_name();
    std::string s = p;
    free(p);
    return s;
}

void ToolLib::parse(const std::string cmd_line, std::string parsed[]) {
    std::stringstream ss(cmd_line);
    int ind = 0;
    parsed[0].clear();
    while (ss >> parsed[ind++]) {
        if (ind == MAX_ARGS) {
            panic("Too many arguments.");
            break;
        }
        parsed[ind].clear();
    }
}

void ToolLib::panic(std::string info) { std::cout << info << std::endl; }

bool ToolLib::is_file_exist(const char *filename) {
    std::ifstream infile(filename);
    return infile.good();
}

bool ToolLib::is_account_exist(const char *account_user_name) {
    std::string fname = cwd() + "/user/" + account_user_name;
    return is_file_exist(fname.c_str());
}

bool ToolLib::is_express_exist(const char *express_ID) {
    std::string fname = cwd() + "/express/" + express_ID;
    return is_file_exist(fname.c_str());
}

void ToolLib::rm_express(const char *express_ID) {
    std::string fpath = tl.cwd() + "/express/" + express_ID;
    remove(fpath.c_str());
}

void ToolLib::bad_cin_handler() {
    if (std::cin.good()) return;
    if (std::cin.eof()) {
        std::cout << "\n exit" << std::endl;
        exit(0);
    } else {
        puts("fail");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

const std::string ToolLib::current_time() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int ToolLib::account_type(std::string nick) {
    std::string fname = cwd() + "/user/" + nick;
    std::ifstream userfile(fname);
    std::string line;
    // first line is type
    getline(userfile, line);
    userfile.close();

    return atoi(line.c_str());
}

int ToolLib::exp_type(std::string express_ID) {
    std::string fname = cwd() + "/express/" + express_ID;
    std::ifstream expfile(fname);
    std::string line;
    // first line is type
    getline(expfile, line);
    expfile.close();

    return atoi(line.c_str());
}
