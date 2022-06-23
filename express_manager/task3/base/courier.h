#pragma once

#include "account.h"
#include "defs.h"

class Courier: public Account {
   public:
    Courier();                      // register
    Courier(std::string nickname);  // log
    
    std::string tele;
    
   private:
    void save_to_file() override;
};
