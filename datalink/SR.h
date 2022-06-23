
/* FRAME kind */
#define FRAME_DATA 1
#define FRAME_ACK 2

/*

struct FRAME {
    unsigned char kind;
    unsigned char ack_seq;  // SEQ(1) or ACK(1)
    unsigned char data[PKT_LEN];
    unsigned int padding;
};

    DATA Frame
    +=========+========+=============+========+
    | KIND(1) | SEQ(1) | DATA(?~256) | CRC(4) |
    +=========+========+=============+========+

    ACK Frame
    +=========+========+========+
    | KIND(1) | ACK(1) | CRC(4) |
    +=========+========+========+
*/
