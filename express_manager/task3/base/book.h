#pragma once
#include "defs.h"
#include "express.h"

class Book : public Express {
   public:
    Book(Account* sender_accoune);
    Book(std::string id) : Express(id) {};

    int getPrice() override;
};
