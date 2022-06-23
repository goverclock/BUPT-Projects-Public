#pragma once
#include "defs.h"
#include "express.h"

class Average : public Express {
   public:
    Average(Account* sender_accoune);
    Average(std::string id) : Express(id) {};

    int getPrice() override;
};
