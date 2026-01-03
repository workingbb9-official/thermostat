#include "app/system_data_handler.h"

#include "logic/uart_mgr.h"
#include "common/protocol.h"

void system_data_handler_init(void) {
    uart_mgr_init();
}

void system_send_temp(int16_t temp) {
    DataPacket temp_packet;
    temp_packet.start_byte = START_BYTE;
    temp_packet.type = TEMP;
    temp_packet.length = 2;

    const uint8_t high_byte = (uint8_t) (temp >> 8);
    const uint8_t low_byte = (uint8_t) (temp & 0xFF);

    temp_packet.payload[0] = high_byte;
    temp_packet.payload[1] = low_byte;
    temp_packet.checksum = 2;

    uart_mgr_transmit(&temp_packet);
}
