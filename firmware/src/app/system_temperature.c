#include "app/system_temperature.h"

#include "logic/therm_mgr.h"
#include "logic/uart_mgr.h"
#include "common/protocol.h"

static uint16_t remove_decimal(float x);
static DataPacket create_temp_packet(uint16_t temp);

void system_send_temp(void) {
    static uint32_t ticks = 0;
    ++ticks;

    if (ticks >= 80000) {
        const float temp_c = therm_mgr_get_temp();
        uint16_t temp_int = remove_decimal(temp_c);
    
        DataPacket temp_packet = create_temp_packet(temp_int);
        uart_mgr_transmit(&temp_packet);
        ticks = 0;
    }
}

/*
 * @brief Removes decimal from float
 * @param Float to be converted
 *
 * Accuracy of 2 decimal places
 * Example: 23.457 turned to 2346
 *
 * @return Converted version of input
 */
static uint16_t remove_decimal(float x) {
    if (x >= 0.0f) {
        return (uint16_t) 100 * x + 0.5f;
    } else {
        return (uint16_t) 100 * x - 0.5f;
    }
}

/*
 * @brief Create a data packet for sending temps
 * @param Temperature reading as an int
 *
 * @return DataPacket for transmitting
 */
static DataPacket create_temp_packet(uint16_t temp) {
    DataPacket temp_packet;
    temp_packet.start_byte = START_BYTE;
    temp_packet.type = TEMP;
    temp_packet.length = 2;
    
    uint8_t high_byte = (temp >> 8);
    uint8_t low_byte = temp & 0xFF;

    temp_packet.payload[0] = high_byte;
    temp_packet.payload[1] = low_byte;
    
    temp_packet.checksum = 2 + 2;
    return temp_packet;
}


