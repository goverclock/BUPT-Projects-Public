#include <limits.h>

#include <algorithm>
#include <iostream>
#include <string>

#include "cmd.h"
#include "defs.h"
#include "tool_lib.h"
#include "account.h"

std::pair<std::string, f> cmd_ls[]{
    std::make_pair("reg", reg),
    std::make_pair("log", log),
    std::make_pair("logout", logout),
    std::make_pair("passwd", passwd),
    std::make_pair("balance", balance),
    std::make_pair("exp", exp),
};

std::map<std::string, f> cmd;
Account *current_account = nullptr;

int main() {
    // establish string-cmd map
    for (auto p : cmd_ls) cmd.insert(p);

    std::string cmd_line;
    std::string args[MAX_ARGS];
    std::string prompt;
    while (1) {
        auto &ca = current_account;
        if (ca == nullptr)
            prompt = "(guest)";
        else if (ca->type == admin)
            prompt = "(" + ca->nickname + "#)";
        else if (ca->type == user)
            prompt = "(" + ca->nickname + "$)";
        std::cout << prompt;

        cmd_line.clear();
        while (cmd_line.empty()) {
            if (!getline(std::cin, cmd_line)) {
                puts("");
                exit(0);
            }
            if (cmd_line.empty()) std::cout << prompt;
        }
        parse(cmd_line, args);

        if (!cmd.count(args[0])) {
            panic(std::string("no such command: ") + args[0]);
            continue;
        }

        cmd[args[0]](args);
    }

    return 0;
}
