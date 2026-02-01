#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define HOME            0x02
#define LOGIN           0x01
#define STATS           0x03
#define START_BYTE      0xAA
#define MAX_PAYLOAD     128
#define PAYLOAD_NONE    0xFF

struct data_packet {
    uint8_t start_byte;
    uint8_t type;
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD];
    uint8_t checksum;
} __attribute__((packed));

#endif // PROTOCOL_H
