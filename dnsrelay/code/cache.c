#include "cache.h"
#include "debug.h"
#include "list.h"

int cache_size;
struct list_head cache_list;
extern char buffer[DNS_MSG_SIZE_LIMIT];
extern char* rrpos;

void cache(struct QUESTION* q, struct RR* rr) {
    struct list_entry* le =
        (struct list_entry*)malloc(sizeof(struct list_entry));
    list_add(&le->list, &cache_list);  // to front
    cache_size++;
    while (cache_size > MAX_CACHE_ENTRY) {
        struct list_entry* dle =
            list_entry(cache_list.prev, struct list_entry, list);
        list_del(cache_list.prev);  // from tail
        free(dle);
        cache_size--;
    }

    strcpy(le->qname, q->qname);
    le->ip = ntohl(rr->rdata);
    le->expire_time = ntohl(*(uint32_t*)&rr->ttl) + time(NULL);
}

int find_cache(struct QUESTION* q, struct RR* rr) {
    if(q->qtype != TYPE_A) return 0;
    
    struct list_head* pos;
    list_for_each(pos, &cache_list) {
        struct list_entry* le = list_entry(pos, struct list_entry, list);
        if (le->expire_time <= time(NULL)) {
            debug(2, "entry out\n");
            list_del(pos);
            cache_size--;
            free(le);
            continue;
        }
        if (!strcmp(q->qname, le->qname)) {
            // found
            rr->type = htons(q->qtype);
            rr->class = htons(q->qclass);
            *(uint32_t*)&rr->ttl = htonl(le->expire_time - time(NULL));
            rr->name = htons(0xc00c);
            rr->rdata = htonl(le->ip);
            rr->rdlength = htons(4);

            list_del(pos);
            list_add(pos, &cache_list);
            return 1;
        }
    }

    return 0;
}

void debug_cache(int level) {
    struct list_head *pos = NULL;
    int i = 0;
    list_for_each(pos, &cache_list) {
        struct list_entry *le = list_entry(pos, struct list_entry, list);
        debug(level, "@%d ", ++i);
        debug_qname(level, le->qname);
        debug(level, " - ");
        debug_ip(level, htonl(le->ip));
        debug(level, "\tttl: %d\n", le->expire_time - time(NULL));
    }
}
