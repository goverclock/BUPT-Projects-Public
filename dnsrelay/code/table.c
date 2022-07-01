#include "table.h"
#include "debug.h"

struct list_entry* table;
int table_size;

// turn " google.com"(note the space at the begining)
// to 06 google 03 com
void to_qname(char* name) {
    char* pos;
    *name = '.';
    while (*name) {
        if (*name == '.') {
            pos = name;
            *pos = 0;
            name++;
        }
        (*pos)++;
        name++;
    }
}

int qname_cmpr(const void* a, const void* b) {
    struct list_entry* p1 = (struct list_entry*)a;
    struct list_entry* p2 = (struct list_entry*)b;
    return strcmp(p1->qname, p2->qname);
}

void table_init(const char* fpath) {
    FILE* fp = fopen(fpath, "r");
    if(fp == NULL) {
        printf("fail to open \"%s\"\n", fpath);
        exit(1);
    }
    int lines = 0;
    while (!feof(fp))
        if (fgetc(fp) == '\n') lines++;
    if(lines > MAX_TABLE_ENTRY) {
        fprintf(stderr, "too many table entries: %d\n", lines);
        exit(1);
    }
    debug(2, "table entrys: %d\n", lines);

    table = (struct list_entry*)malloc(lines * sizeof(struct list_entry));
    memset(table, 0, lines * sizeof(struct list_entry));
    table_size = lines;
    fseek(fp, 0, SEEK_SET);
    char buf[128];
    int ind = 0;
    while (fgets(buf, sizeof(buf), fp)) {
        char name[NAME_SIZE_LIMIT], ip_str[16];
        sscanf(buf, "%s %s", ip_str, name + 1);
        uint32_t ip = inet_addr(ip_str);

        to_qname(name);
        strcpy(table[ind].qname, name);
        table[ind].ip = ntohl(ip);
        table[ind].expire_time = time(NULL) + 157680000;  // seconds of 5 years

        ind++;
    }
    fclose(fp);

    qsort(table, lines, sizeof(struct list_entry), qname_cmpr);
}

int find_table(struct QUESTION* q, struct RR* rr) {
    if (q->qtype != TYPE_A) return 0;

    int l = 0, r = table_size;
    while (l < r) {
        int mid = l + r >> 1;
        int ret = qname_cmpr(q->qname, table[mid].qname);
        if (ret > 0)
            l = mid + 1;
        else
            r = mid;
    }
    if (!qname_cmpr(q->qname, table[l].qname)) {
        // found in table
        rr->type = htons(q->qtype);
        rr->class = htons(q->qclass);
        *(uint32_t*)&rr->ttl = htonl(table[l].expire_time - time(NULL));
        rr->name = htons(0xc00c);
        rr->rdata = htonl(table[l].ip);
        rr->rdlength = htons(4);
        return 1;
    }

    return 0;
}

void debug_table(int level) {
    int limit = 10;
    for (int i = 0; i < limit && i < table_size; i++) {
        debug(level, "$%d ", i + 1);
        debug_qname(level, table[i].qname);
        debug(level, " - ");
        debug_ip(level, htonl(table[i].ip));
        debug(level, "\n");
    }
    if(limit < table_size)
        debug(level, "...\n%d entries omitted\n", table_size - limit);
}
