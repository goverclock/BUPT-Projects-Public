#include "SR.h"

#include <stdio.h>
#include <string.h>

#include "protocol.h"

#define MAX_SEQ 31
#define NR_BUFS ((MAX_SEQ + 1) / 2)

#define DATA_TIMER 1000

struct FRAME {
    unsigned char kind; /* FRAME_DATA */
    unsigned char ack_seq;
    unsigned char data[PKT_LEN];
    unsigned int padding;
};

typedef unsigned char BUFFER[PKT_LEN];

struct BUFFER {
    unsigned char buf[PKT_LEN];
    int len;
};
struct BUFFER in_buf[NR_BUFS], out_buf[NR_BUFS];
static int phl_ready = 0;
static int recv_base = 0;
static int send_base = 0, next_seq_nr = 0;
int acked[MAX_SEQ + 1];
int cached[MAX_SEQ + 1];

static void put_frame(unsigned char *frame, int len) {
    *(unsigned int *)(frame + len) = crc32(frame, len);
    send_frame(frame, len + 4);
    phl_ready = 0;
}

// is b in [a, c]?
static int between(int a, int b, int c) {
    a %= MAX_SEQ + 1;
    b %= MAX_SEQ + 1;
    c %= MAX_SEQ + 1;
    int ret = (((a <= b) && (b <= c)) || ((c <= a) && (a <= b)) ||
                ((b <= c) && (c <= a)));
    return ret;
}

// note: only called when between(send_base, next_seq_nr, send_base + NR_BUFS -
// 1) && phl_ready
void network_layer_ready_handler(int *arg) {
    int len = get_packet(out_buf[next_seq_nr % NR_BUFS].buf);
    out_buf[next_seq_nr % NR_BUFS].len = len;

    // send out immediately and cache it(for possible re-send)
    struct FRAME s;
    s.kind = FRAME_DATA;
    s.ack_seq = next_seq_nr;  // seq
    memcpy(s.data, out_buf[next_seq_nr % NR_BUFS].buf, PKT_LEN);

    dbg_frame("Send DATA %d, ID %d\n", s.ack_seq, *(short *)s.data);

    put_frame((unsigned char *)&s, 2 + len);

    start_timer(s.ack_seq, DATA_TIMER);

    next_seq_nr++;
    next_seq_nr %= MAX_SEQ + 1;
}

void physical_layer_ready_handler(int *arg) { phl_ready = 1; }

void frame_received_handler(int *arg) {
    struct FRAME f;

    int len = recv_frame((unsigned char *)&f, sizeof(f));
    if (len > 6 && crc32((unsigned char *)&f, len) != 0) {
        dbg_event("**** Receiver Error, Bad CRC Checksum\n");
        // ignore the frame
        return;
    }

    // FRAME_ACK:
    if (f.kind == FRAME_ACK &&
        between(send_base, f.ack_seq, send_base + NR_BUFS - 1)) {
        acked[f.ack_seq] = 1;  // ack

        dbg_frame("Recv ACK  %d\n", f.ack_seq);
        stop_timer(f.ack_seq);

        while (acked[send_base]) {
            acked[send_base] = 0;
            send_base++;
            send_base %= MAX_SEQ + 1;
			if(send_base == next_seq_nr) break;
        }
    }

    // FRAME_DATA:
    if (f.kind == FRAME_DATA) {
        dbg_frame("Recv DATA %d, ID %d\n", f.ack_seq, *(short *)f.data);

        // send ack
        struct FRAME s;
        s.kind = FRAME_ACK;
        s.ack_seq = f.ack_seq;  // ack, seq

        dbg_frame("Send ACK  %d\n", s.ack_seq);  // ack
        put_frame((unsigned char *)&s, 2);

        if (between(recv_base, f.ack_seq, recv_base + NR_BUFS - 1)) {
            // cache it, later send to network layer
            if (!cached[f.ack_seq]) {
                memcpy(in_buf[f.ack_seq % NR_BUFS].buf, f.data, len - 6);
                in_buf[f.ack_seq % NR_BUFS].len = len - 6;
                cached[f.ack_seq] = 1;
            }
            while (cached[recv_base]) {
                cached[recv_base] = 0;
                put_packet(in_buf[recv_base % NR_BUFS].buf,
                           in_buf[recv_base % NR_BUFS].len);
                recv_base++;
                recv_base %= MAX_SEQ + 1;
            }
        }
    }
}

void data_timeout_handler(int *arg) {
    int num = *arg;

    struct FRAME s;
    s.kind = FRAME_DATA;
    s.ack_seq = num;
    memcpy(s.data, out_buf[num % NR_BUFS].buf, PKT_LEN);

    put_frame((unsigned char *)&s, 2 + out_buf[num % NR_BUFS].len);
    start_timer(num, DATA_TIMER);

    dbg_frame("Timeout, ReSend DATA %d, ID %d\n", s.ack_seq, *(short *)s.data);
}

void (*event_handler[])(int *) = {
    [NETWORK_LAYER_READY] = network_layer_ready_handler,
    [PHYSICAL_LAYER_READY] = physical_layer_ready_handler,
    [FRAME_RECEIVED] = frame_received_handler,
    [DATA_TIMEOUT] = data_timeout_handler,
};

int main(int argc, char **argv) {
    int event, arg;

    protocol_init(argc, argv);
    lprintf("Designed by Jiang Yanjun, build: " __DATE__
            "  "__TIME__
            "\n");

    disable_network_layer();

    for (;;) {
        event = wait_for_event(&arg);

        event_handler[event](&arg);

        if (between(send_base, next_seq_nr, send_base + NR_BUFS - 1) &&
            phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
    }

    return 0;
}
