#include "account.h"

#include <fstream>

#include "tool_lib.h"
#include "user.h"

extern Account* current_account;

Account::Account() {
    while (1) {
        bad_cin_handler();
        std::cout << "nickname:";
        std::cin >> nickname;

        // check if the user already exists
        if (is_account_exist(nickname.c_str())) {
            std::cout << "user " << nickname << " already exist" << std::endl;
            std::cout << "try another ";
            continue;
        } else
            break;
    }

    while (1) {
        bad_cin_handler();
        std::cout << "password:";
        std::cin >> password;
        std::cout << "repeat password:";
        std::string repeat;
        std::cin >> repeat;
        if (password == repeat) break;

        std::cout << "not the same" << std::endl;
    }

    std::cout << "realname:";
    std::cin >> realname;

    // eat empty line
    std::string t;
    getline(std::cin, t);
}

Account::Account(std::string nick) {
    bool good = true;
    std::string fname = cwd() + "/user/" + nick;
    if (!is_file_exist(fname.c_str()))
        good = false;
    else {
        nickname = nick;
        // read from file
        std::string t;
        std::ifstream userfile(fname);
        getline(userfile, t);
        type = (power)atoi(t.c_str());
        getline(userfile, t);
        password = t;
        getline(userfile, t);
        realname = t;
        getline(userfile, t);
        balance = atoi(t.c_str());

        userfile.close();
    }
}

void Account::passwd(std::string args[]) {
    std::cout << "old password:";
    std::string in;
    std::cin >> in;

    if (in == password) {
        std::cout << "new password:";
        std::cin >> in;
        password = in;
        save_to_file();
        std::cout << "password changed successfully" << std::endl;
    } else
        std::cout << "fail to change password" << std::endl;

    // eat empty line
    getline(std::cin, in);
}

void Account::charge(std::string args[]) {
    if (args[1].empty())
        std::cout << "balance: " << current_account->balance << std::endl;
    else if (args[1] == "charge" && !args[2].empty()) {
        // charge
        int amount = atoi(args[2].c_str());
        current_account->balance += amount;
        save_to_file();

        std::cout << "charged " << amount << std::endl;
        std::cout << "now balance: " << current_account->balance << std::endl;
    } else
        panic("usage: balance [charge <amount>]");
}

void Account::change_balance(int amount) {
    balance += amount;
    save_to_file();
}
