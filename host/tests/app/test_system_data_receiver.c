#include "app/system_data_receiver.h"
#include "common/protocol.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int test_system_receive_data(uint8_t *bytes, DataPacket *packet);

int main(void) {
    DataPacket packet = {0};
    uint8_t bytes[] = {START_BYTE, LOGIN, 2, 15, 20, 3};

    if (test_system_receive_data(bytes, &packet) != 0) {
        printf("Failed\n");
    } else {
        printf("Success\n");
    }
    
    printf("%u\n", packet.start_byte);
    printf("%u\n", packet.type);
    printf("%u\n", packet.length);
    printf("%u\n", packet.payload[0]);
    printf("%u\n", packet.checksum);

    return 0;
}

int test_system_receive_data(uint8_t *bytes, DataPacket *packet) {
    uint8_t first_byte = bytes[0];

    if (first_byte != START_BYTE) {
        return -1;
    }
    packet->start_byte = first_byte;

    uint8_t second_byte = bytes[1];

    switch (second_byte) {
    case LOGIN:
        packet->type = LOGIN;
        break;
    case TEMP:
        packet->type = TEMP;
        break;
    case STATS:
        packet->type = STATS;
        break;
    default:
        return -1;
        break;
    }

    uint8_t third_byte = bytes[2];

    if (third_byte < 1 || third_byte > MAX_PAYLOAD) {
        return -1;
    }
    packet->length = third_byte;
    
    for (uint8_t i = 0; i < packet->length; ++i) {
        uint8_t payload_byte = bytes[i + 3];
        
        packet->payload[i] = payload_byte;
    }
    
    uint8_t checksum_byte = bytes[packet->length + 3];
    packet->checksum = checksum_byte;

    return 0;
}
