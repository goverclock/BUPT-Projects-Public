#include "GBN.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "protocol.h"

#define DATA_TIMER 2000
#define ACK_TIMER 300
#define MAX_SEQ 7
#define inc(num) num = ((num + 1) & MAX_SEQ)

typedef unsigned char seq_nr;

struct FRAME {
    unsigned char kind; /* FRAME_DATA */
    unsigned char ack;
    unsigned char seq;
    unsigned char data[PKT_LEN];
    unsigned int padding;
};

static seq_nr frame_to_send = 0;
static seq_nr ack_expected = 0;
static seq_nr frame_expected = 0;

static bool no_nak = true;
static bool phl_ready = false;

static int frame_length = 0;

static unsigned char buffer[MAX_SEQ + 1][PKT_LEN];
static int packet_length[MAX_SEQ + 1];

static seq_nr nbuffered = 0;

static void put_frame(unsigned char* frame, int len) {
    *(unsigned int*)(frame + len) = crc32(frame, len);

    send_frame(frame, len + 4);
}

static void send_data(seq_nr frame_nr, seq_nr frame_expected,
                      unsigned char* packet, size_t len) {
    struct FRAME s;

    s.kind = FRAME_DATA;
    s.seq = frame_nr;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

    if (len > PKT_LEN) {
        dbg_frame(
            "Error while sending packet %d with ack %d: Length too large.\n",
            s.seq, s.ack, len);
        return;
    }
    memcpy(s.data, packet, PKT_LEN);

    dbg_frame("Packet sent: seq = %d, ack = %d, data id = %d\n", s.seq, s.ack,
              *(short*)s.data);

    // kind + ack + seq + original data
    put_frame((unsigned char*)&s, 3 + PKT_LEN);
}
static void send_ACK(unsigned char frame_expected) {
    struct FRAME s;

    s.kind = FRAME_ACK;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

    dbg_frame(
        "Send "
        "ACK"
        " %d\n",
        s.ack);

    put_frame((unsigned char*)&s, 2);
}
static void send_NAK(unsigned char frame_expected) {
    struct FRAME s;

    s.kind = FRAME_NAK;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

    dbg_frame(
        "Send "
        "NAK"
        " %d\n",
        s.ack);

    put_frame((unsigned char*)&s, 2);
}
static inline bool bewteen(unsigned char a, unsigned char b, unsigned char c) {
    return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) ||
           ((b < c) && (c < a));
}

static void network_ready_handler() {
    // from_network_layer
    packet_length[frame_to_send] = get_packet(buffer[frame_to_send]);
    // expand the sender's window
    nbuffered++;
    // send_data
    send_data(frame_to_send, frame_expected, buffer[frame_to_send],
              packet_length[frame_to_send]);
    start_timer(frame_to_send, DATA_TIMER);
    stop_ack_timer();

    inc(frame_to_send);
    phl_ready = false;
}
static void physical_ready_handler() { phl_ready = true; }
static void frame_received_handler() {
    // from_physical_layer
    struct FRAME f;
    frame_length = recv_frame((unsigned char*)(&f), sizeof(f));
    // bad_frame, should return nak
    if (frame_length < 5 || crc32((unsigned char*)&f, frame_length) != 0) {
        dbg_event("Bad CRC Checksum,Receieve Error!!!\n");
        if (no_nak) {
            send_NAK(frame_expected);
            no_nak = true;
            stop_ack_timer();
        }

        return;
    }
    if (f.kind == FRAME_ACK) dbg_frame("Recv ACK  %d\n", f.ack);
    if (f.kind == FRAME_NAK) dbg_frame("Recv NAK  %d\n", f.ack);
    if (f.kind == FRAME_DATA) {
        dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short*)f.data);
        if (f.seq == frame_expected) {
            put_packet(f.data, frame_length - 7);
            no_nak = true;
            inc(frame_expected);
            start_ack_timer(ACK_TIMER);
        } else if (no_nak) {
            send_NAK(frame_expected);
            no_nak = false;
            stop_ack_timer();
        }
    }

    // stop all the timers before
    while (bewteen(ack_expected, f.ack, frame_to_send)) {
        nbuffered--;
        stop_timer(ack_expected);
        inc(ack_expected);
    }

    if (f.kind == FRAME_NAK) {
        stop_timer(ack_expected + 1);
        frame_to_send = ack_expected;
        // resend those expected packet
        for (seq_nr i = 0; i < nbuffered; i++) {
            send_data(frame_to_send, frame_expected, buffer[frame_to_send],
                      packet_length[frame_to_send]);
            start_timer(frame_to_send, DATA_TIMER);
            stop_ack_timer();

            inc(frame_to_send);
        }

        phl_ready = false;
    }
    return;
}
static void data_timeout_handler(int* arg) {
    dbg_event("---- DATA %d timeout\n", arg);
    frame_to_send = ack_expected;
    for (seq_nr i = 0; i < nbuffered; i++) {
        send_data(frame_to_send, frame_expected, buffer[frame_to_send],
                  packet_length[frame_to_send]);
        start_timer(frame_to_send, DATA_TIMER);
        stop_ack_timer();

        inc(frame_to_send);
    }

    phl_ready = false;
}
static void ACK_timeout_handler() {
    send_ACK(frame_expected);
    stop_ack_timer();
}

void (*event_handler[])(int*) = {
    [NETWORK_LAYER_READY] = network_ready_handler,
    [PHYSICAL_LAYER_READY] = physical_ready_handler,
    [FRAME_RECEIVED] = frame_received_handler,
    [DATA_TIMEOUT] = data_timeout_handler,
    [ACK_TIMEOUT] = ACK_timeout_handler,
};

int main(int argc, char** argv) {
    int event, arg;

    protocol_init(argc, argv);
    disable_network_layer();
    for (;;) {
        event = wait_for_event(&arg);

        event_handler[event](&arg);

        if (nbuffered < MAX_SEQ && phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
    }

    return 0;
}
