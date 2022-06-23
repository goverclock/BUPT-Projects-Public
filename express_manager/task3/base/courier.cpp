#include "courier.h"

#include <fstream>

#include "tool_lib.h"

extern ToolLib tl;

// register
Courier::Courier() {
    type = courier;

    std::cout << "tele:";
    std::cin >> tele;

    // eat empty line
    std::string t;
    getline(std::cin, t);

    save_to_file();
}

Courier::Courier(std::string nick) : Account(nick) {
    nickname = nick;

    std::string fname = tl.cwd() + "/user/" + nick;
    std::string line;
    std::ifstream userfile(fname);
    getline(userfile, line);
    tele = line;
    userfile.close();
}

void Courier::save_to_file() {
    std::string fname = tl.cwd() + "/user/" + nickname;
    std::ofstream outfile(fname);
    outfile << type << std::endl;
    outfile << password << std::endl;
    outfile << realname << std::endl;
    outfile << balance << std::endl;
    outfile << tele << std::endl;
    outfile.close();
}
