#include "average.h"
#include <fstream>

Average::Average(Account *sender_account) : Express(sender_account) {
    type = average;

    save_to_file(); 
}

int Average::getPrice() { return 5 * weight; }

