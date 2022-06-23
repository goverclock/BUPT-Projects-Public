#pragma once
#include <unistd.h>

#include <string>

#include "tool_lib.h"

struct Command {
    void reg(std::string args[]);
    void log(std::string args[]);
    void logout(std::string args[]);
    void passwd(std::string args[]);
    void balance(std::string args[]);
    void exp(std::string args[]);
    void accmgr(std::string args[]);
};
