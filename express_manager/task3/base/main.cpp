// base/ is totally same as task2/

#include <limits.h>

#include <algorithm>
#include <iostream>
#include <string>

#include "account.h"
#include "cmd.h"
#include "defs.h"
#include "tool_lib.h"

std::map<std::string, f> cmd_map;
Account *current_account = nullptr;
ToolLib tl;
Command cmd;

int main() {
    // establish string-cmd_map map
    std::pair<std::string, f> cmd_ls[]{
        std::make_pair("reg", &Command::reg),
        std::make_pair("log", &Command::log),
        std::make_pair("logout", &Command::logout),
        std::make_pair("passwd", &Command::passwd),
        std::make_pair("balance", &Command::balance),
        std::make_pair("exp", &Command::exp),
        std::make_pair("accmgr", &Command::accmgr),
    };
    for (auto p : cmd_ls) cmd_map.insert(p);

    std::string cmd_line;
    std::string args[MAX_ARGS];
    std::string prompt;
    while (1) {
        auto &ca = current_account;
        if (ca == nullptr)
            prompt = "(guest)";
        else if (ca->type == admin)
            prompt = "(" + ca->nickname + "#)";
        else if (ca->type == user || ca->type == courier)
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
        tl.parse(cmd_line, args);

        if (!cmd_map.count(args[0])) {
            tl.panic(std::string("no such command: ") + args[0]);
            continue;
        }

        // see Stack Overflow for this part:
        // https://stackoverflow.com/questions/14419202/c-map-of-string-and-member-function-pointer
        f myf = cmd_map[args[0]];
        (cmd.*myf)(args);
    }

    return 0;
}
