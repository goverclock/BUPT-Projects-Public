#include "user.h"

#include <fstream>
#include <iostream>

#include "tool_lib.h"

extern Account *current_account;

// register a new user
User::User() {
    type = user;

    std::cout << "tele:";
    std::cin >> tele;
    std::cout << "address:";
    std::cin >> address;

    // eat empty line
    std::string t;
    getline(std::cin, t);

    save_to_file();
}

// log in an existening user from file
User::User(std::string nick) : Account(nick) {
    nickname = nick;
    // read from file
    std::string fname = cwd() + "/user/" + nick;
    std::string t;
    std::ifstream userfile(fname);
    getline(userfile, t);
    tele = t;
    getline(userfile, t);
    address = t;

    userfile.close();
}

void User::save_to_file() {
    // save to file cwd/user/nickname
    // format: see handin/report.md
    std::string fname = cwd() + "/user/" + nickname;
    std::ofstream outfile(fname);
    outfile << type << std::endl;      // type
    outfile << password << std::endl;  // password
    outfile << realname << std::endl;  // real name
    outfile << balance << std::endl;   // balance
    outfile << tele << std::endl;      // tele
    outfile << address << std::endl;   // address
    outfile.close();
}
