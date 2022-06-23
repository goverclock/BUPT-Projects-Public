#include "book.h"
#include <fstream>

Book::Book(Account *sender_account) : Express(sender_account) {
    type = book;

    save_to_file(); 
}

int Book::getPrice() { return 2 * weight; }
