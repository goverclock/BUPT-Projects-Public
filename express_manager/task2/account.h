#pragma once
#include <cstdlib>
#include <iostream>
#include <string>

#include "defs.h"

class Account {
   public:
    Account();                  // register, save to file
    Account(std::string nick);  // log, read from file

    user_type type;
    std::string nickname;
    std::string realname;
    std::string password;
    int balance = 0;

    void passwd(std::string args[]);
    void charge(std::string args[]);

    bool change_balance(int, bool);
   private:
    virtual void save_to_file() = 0;
};
