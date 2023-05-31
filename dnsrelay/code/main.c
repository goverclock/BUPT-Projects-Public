#include "cache.h"
#include "debug.h"
#include "defs.h"
#include "table.h"
#include "trans.h"

char buffer[DNS_MSG_SIZE_LIMIT];

struct sockaddr_in any_in_adr, dns_addr;
char dns_server[16] = DNS_IP;
char table_file[256] = TABLE_FILE;

extern struct requset_info requests[1 << 16];
extern struct list_head cache_list;
extern struct list_entry *table;
extern char *optarg;  // just shut vscode up

int debug_level = 0;
time_t epoch;

char *question_init(struct QUESTION *, char *);
void argument_resolv(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    argument_resolv(argc, argv);
    socket_init();
    table_init(table_file);
    list_init(&cache_list);
    time(&epoch);
    memset(requests, 0, sizeof(requests));

    debug_table(2);
    while (1) {
        int any_in_adr_len = sizeof(any_in_adr);
        int msg_len = recvfrom(sock, buffer, DNS_MSG_SIZE_LIMIT, 0,
                               (struct sockaddr *)&any_in_adr, &any_in_adr_len);
        if (msg_len <= 0) continue;

        // new message
        // UDP segment has boundary, so this must be a single DNS message
        struct HEADER *header = (struct HEADER *)buffer;
        struct QUESTION question;
        char *rrpos = question_init(&question, buffer + sizeof(struct HEADER));

        debug_time(1);
        debug(1, " %c\t", header->qr ? 'A' : 'Q');
        debug(1, " from: ");
        debug(1, "%-16s\t", inet_ntoa(any_in_adr.sin_addr));

        if (header->qr == 0) {  // question from local progress
            struct RR rr;
            int found = 0;
            if (!(found |= find_cache(&question, &rr)))
                found |= find_table(&question, &rr) << 1;

            if (found) {
                header->qr = 1;
                if (rr.rdata == 0)  // domain has no ip
                    header->rcode = 3;
                else
                    header->rcode = 0;
                header->ancount = htons(1);
                msg_len += sizeof(struct RR);
                memcpy(rrpos, (char *)&rr, sizeof(struct RR));
                sendto(sock, buffer, msg_len, 0, (struct sockaddr *)&any_in_adr,
                       sizeof(any_in_adr));

                // found in table, save to cache
                if (found & 1 << 1) cache(&question, &rr);
            } else {
                // ask DNS server
                int origin_id = header->id;
                header->id = save_request(&any_in_adr, header->id);
                sendto(sock, buffer, msg_len, 0, (struct sockaddr *)&dns_addr,
                       sizeof(dns_addr));

                debug(1, "ID: %-5d -> %-5d\ttype: ", origin_id, header->id);
                if (question.qtype == TYPE_A)
                    debug(1, "A\t\t");
                else
                    debug(1, "%-3d\t", question.qtype);
            }

            if (!found) {
                debug_qname(1, question.qname);
                debug(1, " -> ?");
            } else if (found & 1) {
                debug(1, "found in cache\t\ttype: null\t");
                debug_qname(1, question.qname);
                debug(1, " -> ? <- ");
                debug_ip(1, rr.rdata);
            } else if (found & (1 << 1)) {
                debug(1, "found in table\t\ttype: null\t");
                debug_qname(1, question.qname);
                debug(1, " -> ? <- ");
                debug_ip(1, rr.rdata);
            }
        } else {  // answer from DNS server
            struct sockaddr_in local_adr;
            int serv_id = header->id;
            header->id = acquire_request(&local_adr, header->id);
            if (local_adr.sin_addr.s_addr == 0) {  // answer is late
                debug(1, "\n");
                continue;
            }
            sendto(sock, buffer, msg_len, 0, (struct sockaddr *)&local_adr,
                   sizeof(local_adr));

            // cache
            // only caches messages that are
            // 1. standard request with exactly 1 question
            // 2. A type
            if (ntohs(header->opcode) != 0 || ntohs(header->qdcount) != 1) {
                debug(1, "\n");
                continue;
            }

            struct RR *rr = (struct RR *)rrpos;
            if (ntohs(rr->type) == TYPE_A) cache(&question, rr);

            debug(1, "ID: %-5d <- %-5d\t", header->id, serv_id);
            if (header->rcode != 0) {
                debug(1, "type: null\t");
                debug_qname(1, question.qname);
                debug(1, " <- ");
                debug(1, "no such domain");
            } else {
                if (ntohs(rr->type) == TYPE_A)
                    debug(1, "type: A\t\t");
                else
                    debug(1, "type: %-3d\t", ntohs(rr->type));

                debug_qname(1, question.qname);
                debug(1, " <- ");
                if (ntohs(rr->type) == TYPE_A)
                    debug_ip(1, rr->rdata);
                else
                    debug(1, "...");
            }
        }
        debug(1, "\n");
        debug_cache(2);
    }

    socket_close(sock);
    free(table);
    struct list_head *pos, *n;
    for (pos = cache_list.next, n = pos->next; pos != &cache_list;
         pos = n, n = pos->next) {
        struct list_entry *le = list_entry(pos, struct list_entry, list);
        list_del(pos);
        free(le);
    }

    return 0;
}

char *question_init(struct QUESTION *q, char *buf) {
    strcpy(q->qname, buf);
    char *pos = buf + strlen(buf) + 1;
    uint16_t val;
    memcpy(&val, pos, 2);
    q->qtype = ntohs(val);
    memcpy(&val, pos + 2, 2);
    q->qclass = ntohs(val);
    return pos + 2 + 2;
}

void argument_resolv(int argc, char *argv[]) {
    char opt;
    while ((opt = getopt(argc, argv, "d::a:f:")) != 255) {
        if (opt == 'd') {
            if (optarg == NULL)
                debug_level = 1;
            else if (*optarg == 'd')
                debug_level = 2;
            else {
                printf("%s: invalid option -- \'-d%s\'\n", argv[0], optarg);
                exit(1);
            }
        } else if (opt == 'a')
            strcpy(dns_server, optarg);
        else if (opt == 'f')
            strcpy(table_file, optarg);
        else {
            printf(
                "usage: [-d | -dd] [-a <dns-server-ipaddr>] [-f "
                "<filename>]\n");
            printf("\t-d\t\t\t\tset debug level to 1\n");
            printf("\t-dd\t\t\t\tset debug level to 2\n");
            printf("\t-a <dns-server-ipaddr>\t\tspecify an DNS server\n");
            printf("\t-f <filename>\t\t\tspecify a table file\n");
            exit(1);
        }
    }
}
