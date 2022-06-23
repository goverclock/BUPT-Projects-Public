#pragma once
#include "tool_lib.h"
#include <iostream>
#include <string>
#include "defs.h"
#include "account.h"

class Express {
   public:
    Express(Account* sender_account);    // create and save to file
    Express(std::string id);        // read from file

    std::string ID;

    std::string sender;
    std::string receiver;
    express_status status;
    std::string send_time;
    std::string receive_time;   // = null -> not received yet
    std::string description;

    void ls_info(Account*, Filter);
    void accept();

   private:
    std::string generate_ID();
    void save_to_file();
};
