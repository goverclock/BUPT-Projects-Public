#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

class Reader {
  public:
    Reader(std::string ifname, std::string ofname) : ifs(ifname), ofs(ofname) {
        assert(ifname != ofname);
        assert(ifs.is_open());
        assert(ofs.is_open());
    }

    bool getchar(char &);
    char next_char();

    void out(std::string);

    std::string cur_seg;
    unsigned total_lines = 1;
    unsigned total_char = 0;
  private:
    std::ifstream ifs;
    std::ofstream ofs;
};
