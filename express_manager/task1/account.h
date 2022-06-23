#pragma once
#include <cstdlib>
#include <iostream>
#include <string>

#include "defs.h"

class Account {
   public:
    Account();                  // register, save to file
    Account(std::string nick);  // log, read from file

    power type;
    std::string nickname;
    std::string realname;
    std::string password;
    int balance = 0;

    void passwd(std::string args[]);
    void charge(std::string args[]);

    void change_balance(int);
   private:
    virtual void save_to_file() = 0;
};
