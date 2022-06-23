#pragma once
#include "account.h"
#include "defs.h"

class Admin : public Account {
   public:
    Admin();                      // register, create a new admin
    Admin(std::string nickname);  // log, read from file

    std::string company;

   private:
    void save_to_file() override;
};
