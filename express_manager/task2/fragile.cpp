#include "fragile.h"
#include <fstream>

Fragile::Fragile(Account *sender_account) : Express(sender_account) {
    type = fragile;

    save_to_file(); 
}

int Fragile::getPrice() { return 8 * weight; }

