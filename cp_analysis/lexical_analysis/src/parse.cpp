#include "../include/parse.h"

#include <map>

#include "../include/defs.h"

void Parse::run() {
    char ch;
    std::string current;
    while (1) {
        if (!reader.getchar(ch)) {
            break;
        }
        current.clear();
        current += ch;

        // blank
        if (is_blank(ch)) {
            continue;
        }
        // pre-processing
        if (ch == '#') {
            eat_line();
            continue;
        }
        // in line comment
        if (ch == '/' && reader.next_char() == '/') {
            auto c = eat_line();
            save_comment("/" + c);
            continue;
        }
        // block comment
        if (ch == '/' && reader.next_char() == '*') {
            while (reader.getchar(ch) || is_blank(ch)) {
                current += ch;
                if (ch == '*' && reader.next_char() == '/') {
                    reader.getchar(ch);
                    current += ch;
                    save_comment(current);
                    break;
                }
            }
            continue;
        }
        // identifier
        if (ch == '_' || isalpha(ch)) {
            auto nc = reader.next_char();
            while (nc == '_' || isalpha(nc) || isdigit(nc)) {
                reader.getchar(ch);
                current += ch;
                nc = reader.next_char();
            }
            if (!is_reserve(current)) {
                save_token("id", current);
                save_symbol(current);
            } else {
                save_token(current);
            }
            continue;
        }
        // punctuation
        if (is_punctuation(current)) {
            while (is_punctuation(current + reader.next_char())) {
                reader.getchar(ch);
                current += ch;
            }
            if (current == "=") {
                save_token("assign-op", current);
            } else if (is_relop(current)) {
                save_token("relop", current);
            } else {
                save_token(current);
            }
            continue;
        }
        // constant: integer/float number
        if (isdigit(ch)) {
            while (isdigit(reader.next_char())) {
                reader.getchar(ch);
                current += ch;
            }
            if (reader.next_char() == '.') {  // float
                reader.getchar(ch);
                current += ch;
                while (isdigit(reader.next_char())) {
                    reader.getchar(ch);
                    current += ch;
                }
            }  // else integer
            if (current.back() == '.') {
                save_error("bad punctuation \'.\'");
                continue;
            }
            save_symbol(current);
            save_token("constant", current);
            continue;
        }
        // constant: char
        if (ch == '\'') {
            reader.getchar(ch);
            current += ch;
            reader.getchar(ch);
            current += ch;
            if (ch != '\'') {
                save_error("bad \'\'");
            } else {
                save_symbol(current);
                save_token("constant", current);
            }
            continue;
        }
        // constant: string
        if (ch == '\"') {
            while (reader.next_char() != '\"') {
                if (!reader.getchar(ch) || ch == '\n') {
                    break;
                }
                current += ch;
                if (ch == '\n') {
                    save_error("bad \"\"");
                    continue;
                }
            }
            if (reader.next_char() == '\"') {
                reader.getchar(ch);
                current += ch;
                save_symbol(current);
                save_token("constant", current);
            } else {
                save_error("bad \"\"");
            }
            continue;
        }

        // default: unknown error
        save_error("unknowd token: " + current);
    }
}

void Parse::output_result() {
    // total
    reader.out("Total lines: ");
    reader.out(std::to_string(reader.total_lines));
    reader.out("\n");

    reader.out("Total characters: ");
    reader.out(std::to_string(reader.total_char));
    reader.out("\n\n");

    // symbols
    reader.out("|---------------Symbols---------------|\n");
    for (const auto &s : symblos) {
        reader.out(s + "\n");
    }
    reader.out("\n\n");

    // tokens
    reader.out("|---------------Tokens----------------|\n");
    std::map<std::string, unsigned> m;
    for (const auto &t : tokens) {
        reader.out("<" + t.first + ", " + t.second + ">\n");
    }
    reader.out("\n");
    reader.out("statistics:\n");
    reader.out("constant: " + std::to_string(constant_count) + "\n");
    reader.out("id: " + std::to_string(id_count) + "\n");
    reader.out("reserved: " + std::to_string(reserved_count) + "\n");
    reader.out("\n\n");

    // comments
    reader.out("|--------------Comments---------------|\n");
    for (const auto &c : comments) {
        reader.out(c + "\n");
    }
    reader.out("\n\n");

    // // errors
    reader.out("|---------------Errors----------------|\n");
    for (const auto &e : errors) {
        reader.out(e + "\n");
    }
    reader.out("\n\n");
}

void Parse::save_token(std::string token, std::string attr) {
    if (token == "constant") {
        constant_count++;
    } else if (token == "id") {
        id_count++;
    } else if (attr == "-" && is_reserve(token)) {
        reserved_count++;
    }
    tokens.emplace_back(std::make_pair(token, attr));
}

void Parse::save_symbol(std::string s) { symblos.emplace_back(s); }

void Parse::save_comment(std::string c) { comments.emplace_back(c); }

void Parse::save_error(std::string e) {
    e = "Line " + std::to_string(reader.total_lines) + ": " + e;
    errors.emplace_back(e);
}

std::string Parse::eat_line() {
    char ch;
    std::string append;
    while (reader.getchar(ch)) {
        if (ch == '\n') {
            break;
        }
        append += ch;
    }
    return append;
}

bool Parse::is_reserve(const std::string &s) {
    for (const auto &i : reserve) {
        if (s == i) {
            return true;
        }
    }
    return false;
}

bool Parse::is_punctuation(const std::string &s) {
    for (const auto &i : punctuations) {
        if (s == i) {
            return true;
        }
    }
    return false;
}

bool Parse::is_blank(const char &c) {
    char b[] = {' ', '\t', '\n'};
    for (const auto &i : b) {
        if (c == i) {
            return true;
        }
    }
    return false;
}

bool Parse::is_relop(const std::string &s) {
    for (const auto &i : relops) {
        if (s == i) {
            return true;
        }
    }
    return false;
}