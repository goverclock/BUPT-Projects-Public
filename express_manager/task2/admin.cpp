#include "admin.h"

#include <fstream>
#include <iostream>

#include "tool_lib.h"

extern Account *current_account;
extern ToolLib tl;

// register a new user
Admin::Admin() {
    type = admin;

    std::cout << "company: ";
    std::cin >> company;
    std::cout << "register successfully" << std::endl;

    // eat empty line
    std::string t;
    getline(std::cin, t);

    save_to_file();
}

// log in an existening user from file
Admin::Admin(std::string nick) : Account(nick) {
    nickname = nick;
    // read from file
    std::string fname = tl.cwd() + "/user/" + nick;
    std::string t;
    std::ifstream userfile(fname);

    getline(userfile, t);
    company = t;

    userfile.close();
}

void Admin::save_to_file() {
    // save to file tl.cwd/user/nickname
    // format: see handin/report.md
    std::string fname = tl.cwd() + "/user/" + nickname;
    std::ofstream outfile(fname);
    outfile << type << std::endl;      // type
    outfile << password << std::endl;  // password
    outfile << realname << std::endl;  // real name
    outfile << balance << std::endl;   // balance
    outfile << company << std::endl;   // company
    outfile.close();
}
