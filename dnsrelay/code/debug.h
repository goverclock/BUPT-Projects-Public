#pragma once
#include "defs.h"

void debug(int level, const char *fmt, ...);
void debug_time(int level);
void debug_qname(int level, char* qname);
void debug_ip(int level, uint32_t rdata);
unsigned int get_ms();
