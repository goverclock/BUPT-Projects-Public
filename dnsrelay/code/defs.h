#pragma once

#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

static int sock;
#elif _WIN32
#include <WinSock2.h>
WSADATA wsadata;
SOCKET sock;
#endif

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "list.h"

#define MAX_REQUEST 1 << 16
#define REQ_OUT_TIME 60
#define DNS_IP "114.114.114.114"
#define TABLE_FILE "dnsrelay.txt"
#define MAX_CACHE_ENTRY 64
#define MAX_TABLE_ENTRY 1024

#define TYPE_A 1  // other types(e.g. CNAME) are not considered

#define DNS_MSG_SIZE_LIMIT 512  // see RFC1035, 2.3.4 for the limits
#define NAME_SIZE_LIMIT 256

struct HEADER {
    unsigned id : 16;    /* query identification number */
    unsigned rd : 1;     /* recursion desired */
    unsigned tc : 1;     /* truncated message */
    unsigned aa : 1;     /* authoritive answer */
    unsigned opcode : 4; /* purpose of message */
    unsigned qr : 1;     /* response flag */
    unsigned rcode : 4;  /* response code */
    unsigned cd : 1;     /* checking disabled by resolver */
    unsigned ad : 1;     /* authentic data from named */
    unsigned z : 1;      /* unused bits, must be ZERO */
    unsigned ra : 1;     /* recursion available */
    uint16_t qdcount;    /* number of question entries */
    uint16_t ancount;    /* number of answer entries */
    uint16_t nscount;    /* number of authority entries */
    uint16_t arcount;    /* number of resource entries */
};                       // 12 bytes in total

struct QUESTION {
    char qname[NAME_SIZE_LIMIT];
    uint16_t qtype;
    uint16_t qclass;
};

struct RR {
    uint16_t name;
    uint16_t type;
    uint16_t class;
    uint16_t ttl;  // memory alignment
    uint16_t ttl_;
    uint16_t rdlength;
    uint32_t rdata;  // hexadecimal IP for A query
};

struct list_entry {
    char qname[NAME_SIZE_LIMIT];
    uint32_t ip;
    time_t expire_time;
    struct list_head list;
};

extern struct sockaddr_in any_in_adr, dns_addr;
extern char dns_server[16];

static void socket_init() {
#ifdef _WIN32
    int sta = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (sta != 0) {
        fprintf(stderr, "WSAStartup() failed\n");
        exit(1);
    }
#endif

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        fprintf(stderr, "socket() failed\n");
        exit(1);
    }

    memset(&any_in_adr, 0, sizeof(any_in_adr));
    any_in_adr.sin_family = AF_INET;
    any_in_adr.sin_addr.s_addr = INADDR_ANY;
    any_in_adr.sin_port = htons(53);

    memset(&dns_addr, 0, sizeof(dns_addr));
    dns_addr.sin_family = AF_INET;
    dns_addr.sin_addr.s_addr = inet_addr(dns_server);
    dns_addr.sin_port = htons(53);

    int ret = bind(sock, (struct sockaddr *)&any_in_adr, sizeof(any_in_adr));
    if (ret < 0) {
        fprintf(stderr, "bind() failed\n");
        exit(1);
    }
}

static void socket_close(int sock) {
#ifdef __linux__
    close(sock);
#elif _WIN32
    closesocket(sock);
    WSACleanup();
#endif
}
