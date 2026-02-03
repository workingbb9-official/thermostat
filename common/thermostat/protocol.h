#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define START_BYTE      0xAA

/* Packet types */
#define LOGIN           0x01
#define LOGOUT          0x02
#define TEMP            0x03
#define STATS           0x04
#define WEATHER         0x05
#define CONDITION       0x06

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
