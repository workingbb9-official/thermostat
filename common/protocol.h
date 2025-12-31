#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define MAX_PAYLOAD 128
#define START_BYTE 0xAA
#define LOGIN 0x01
#define TEMP 0x02
#define STATS 0x03

typedef struct __attribute__((packed)) DataPacket {
    uint8_t start_byte;
    uint8_t type;
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD];
    uint8_t checksum;
} DataPacket;

#endif // PROTOCOL_H
