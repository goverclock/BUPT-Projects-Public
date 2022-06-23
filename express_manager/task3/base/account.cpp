#include "account.h"

#include <fstream>

#include "tool_lib.h"
#include "user.h"

extern Account* current_account;
extern ToolLib tl;

Account::Account() {
    while (1) {
        tl.bad_cin_handler();
        std::cout << "nickname:";
        std::cin >> nickname;

        // check if the user already exists
        if (tl.is_account_exist(nickname.c_str())) {
            std::cout << "user " << nickname << " already exist" << std::endl;
            std::cout << "try another ";
            continue;
        } else
            break;
    }

    while (1) {
        tl.bad_cin_handler();
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

// make sure this nick exist
Account::Account(std::string nick) {
    std::string fname = tl.cwd() + "/user/" + nick;
    if (tl.is_file_exist(fname.c_str())) {
        nickname = nick;
        // read from file
        std::string t;
        std::ifstream userfile(fname);
        getline(userfile, t);
        type = (user_type)atoi(t.c_str());
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
        if(amount < 0) {
            std::cout << "bad input" << std::endl;
            return;
        }
        current_account->balance += amount;
        save_to_file();

        std::cout << "charged " << amount << std::endl;
        std::cout << "now balance: " << current_account->balance << std::endl;
    } else
        tl.panic("usage: balance [charge <amount>]");
}

// return false if fail
bool Account::change_balance(int amount, bool echo) {
    if (balance + amount < 0) return false;
    balance += amount;
    save_to_file();
    if(!echo) return true;
    if (amount >= 0)
        std::cout << "balance +" << amount << " ,remaining: " << balance
                  << std::endl;
    else
        std::cout << "balance " << amount << " ,remaining: " << balance
                  << std::endl;
    return true;
}
