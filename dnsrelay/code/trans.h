#pragma once
#include "defs.h"

struct requset_info {
    int used;
    time_t begin_time;
    uint16_t origin_id;
    uint32_t ip;
    uint16_t port;
};

uint16_t save_request(struct sockaddr_in* src, uint16_t origin_id);
uint16_t acquire_request(struct sockaddr_in * dst, uint16_t trans_id);
