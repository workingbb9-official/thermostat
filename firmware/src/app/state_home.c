#include "app/state_home.h"

#include <stdint.h>

#include "thermistor/thermistor.h"
#include "uart/uart.h"
#include "lcd/lcd.h"
#include "common/protocol.h"

static int16_t format_temp(float temp);
static struct DataPacket create_temp_packet(int16_t temp_int);

void home_run(enum sys_state *current_state) {
    static volatile uint32_t temp_timer = 0;
    ++temp_timer;

    if (temp_timer >= TEMP_DELAY) {
        const float temp = therm_mgr_get_temp();
        const int16_t temp_int = format_temp(temp);
        const struct DataPacket temp_packet = create_temp_packet(temp_int);
        uart_mgr_transmit(&temp_packet);

        lcd_mgr_clear();
        lcd_mgr_write("Temp: ");
        lcd_mgr_write_int(temp_int / 100); // High part
        lcd_mgr_write(".");
        lcd_mgr_write_int(temp_int % 100); // Low part


        temp_timer = 0;
        *current_state = STATE_HOME;
    }
}

static int16_t format_temp(float temp) {
    if (temp > 0.0f) {
        return (int16_t) 100.0f * temp + 0.5f;
    } else {
        return (int16_t) 100.0f * temp - 0.5f;
    }
}

static struct DataPacket create_temp_packet(int16_t temp_int) {
    struct DataPacket temp_packet;
    temp_packet.start_byte = START_BYTE;
    temp_packet.type = TEMP;
    temp_packet.length = 2;

    uint8_t high_byte = (uint8_t) temp_int >> 8;
    uint8_t low_byte = (uint8_t) temp_int & 0xFF;
    temp_packet.payload[0] = high_byte;
    temp_packet.payload[1] = low_byte;

    temp_packet.checksum = 2;
    return temp_packet;
}
