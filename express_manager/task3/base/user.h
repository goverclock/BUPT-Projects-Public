#pragma once
#include "account.h"
#include "defs.h"

class User : public Account {
   public:
    User();                      // register, create a new user
    User(std::string nickname);  // log, read from file

    std::string tele;
    std::string address;

   private:
    void save_to_file() override;
};
