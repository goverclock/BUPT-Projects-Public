#pragma once
#include "defs.h"
#include "list.h"

void table_init(const char* fpath);
void to_qname(char*);
int find_table(struct QUESTION*, struct RR*);
void debug_table(int level);
