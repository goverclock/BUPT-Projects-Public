#pragma once
#include <string>

const std::string reserve[]{
    "break",    "case",   "char",     "const",  "continue", "default", "do",
    "double",   "else",   "enum",     "extern", "float",    "for",     "goto",
    "if",       "inline", "int",      "long",   "register", "return",  "short",
    "signed",   "sizeof", "static",   "struct", "switch",   "typedef", "union",
    "unsigned", "void",   "volatile", "while"};

const std::string punctuations[]{
    "[",  "]",  "(",  ")",  "{",  "}",   ".",   "->", "++", "--", "&",   "*",
    "+",  "-",  "~",  "!",  "/",  "%",   "<<",  ">>", "<",  ">",  "<=",  ">=",
    "==", "!=", "^",  "|",  "&&", "||",  "?",   ":",  ",",  ";",  "...", "=",
    "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|="};

const std::string relops[]{"<", "<=", "==", ">", ">=", "!="};