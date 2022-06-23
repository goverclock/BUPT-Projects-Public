#pragma once
#include "defs.h"
#include "express.h"

class Fragile : public Express {
   public:
    Fragile(Account* sender_accoune);
    Fragile(std::string id) : Express(id) {};

    int getPrice() override;
};
