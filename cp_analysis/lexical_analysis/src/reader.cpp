#include "../include/reader.h"

bool Reader::getchar(char &c) {
    if (!ifs.get(c)) {
        return false;
    }
    total_char++;
    if (c == '\n') {
        total_lines++;
    }
    return true;
}

char Reader::next_char() {
    return ifs.peek();
}

void Reader::out(std::string s) {
    ofs << s;
    std::cout << s;
}
