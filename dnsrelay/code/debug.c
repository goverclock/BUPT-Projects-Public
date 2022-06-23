#include "debug.h"

#include <stdarg.h>

#include "list.h"

extern int debug_level;
extern time_t epoch;

void debug(int level, const char *fmt, ...) {
    if (debug_level < level) return;
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void debug_time(int level) {
    if (debug_level < level) return;
    unsigned int ms = get_ms();
    char timestamp[128];
    sprintf(timestamp, "%03d.%03d", ms / 1000, ms % 1000);
    debug(level, "%s ", timestamp);
}

void debug_qname(int level, char *qname) {
    if (debug_level < level) return;
    // assuming that there is only one question
    int nbytes;
    while (1) {
        nbytes = (int)*qname;
        qname++;
        while (nbytes--) {
            printf("%c", *qname);
            qname++;
        }
        if (*qname != '\0')
            printf(".");
        else
            break;
    }
}

void debug_ip(int level, uint32_t rdata) {
    if (debug_level < level) return;
    struct in_addr a;
    a.s_addr = rdata;
    debug(level, "%s", (char *)inet_ntoa(a));
}

unsigned int get_ms() {
    struct timeval tm;
    gettimeofday(&tm, NULL);
    return (unsigned int)((tm.tv_sec - epoch) * 1000 + tm.tv_usec / 1000);
}
