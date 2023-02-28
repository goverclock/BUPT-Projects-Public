#pragma once
#include <string>
#include <vector>

#include "../include/reader.h"

class Parse {
   public:
    Parse(Reader &r) : reader(r){};

    void run();
    void output_result();

   private:
    void save_token(std::string token, std::string attr = "-");
    void save_symbol(std::string);
    void save_comment(std::string);
    void save_error(std::string);
    std::string eat_line();

    bool is_reserve(const std::string&);
    bool is_punctuation(const std::string&);
    bool is_blank(const char &c);
    bool is_relop(const std::string&);

    Reader &reader;
    std::vector<std::pair<std::string, std::string>> tokens;
    std::vector<std::string> symblos;
    std::vector<std::string> comments;
    std::vector<std::string> errors;
    // tokens count by type
    unsigned constant_count = 0;
    unsigned id_count = 0;
    unsigned reserved_count = 0;
};
