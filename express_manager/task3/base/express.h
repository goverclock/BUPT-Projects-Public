#pragma once
#include "tool_lib.h"
#include <iostream>
#include <string>
#include "defs.h"
#include "account.h"

class Fragile;
class Book;
class Average;

class Express {
    friend Fragile;
    friend Book;
    friend Average;
   public:
    Express(Account* sender_account);    // create and save to file
    Express(std::string id);        // read from file

    express_type type;
    std::string ID;

    std::string sender;
    std::string receiver;
    express_status status;
    std::string send_time;
    std::string receive_time;   // = null -> not received yet
    std::string description;
    int weight;

    std::string courier_nick;

    void ls_info(Account*, Filter);
    void accept();
    bool assign_to(std::string courier_nick);
    void delivery();
    virtual int getPrice() = 0;

   private:
    std::string generate_ID();
    void save_to_file();
};
