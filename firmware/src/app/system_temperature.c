#include "app/system_temperature.h"

#include "logic/therm_mgr.h"
#include "logic/uart_mgr.h"
#include "logic/lcd_mgr.h"
#include "common/protocol.h"
#include <util/delay.h>

#define DELAY_TIME 1250000

static int16_t remove_decimal(float x);
static DataPacket create_temp_packet(uint16_t temp);
static void itoa(char *string, int16_t value);

void system_send_temp(void) {
    static volatile uint32_t ticks = 0;
    ++ticks;

    if (ticks >= DELAY_TIME) {
        const float temp_c = therm_mgr_get_temp();
        const int16_t temp_int = remove_decimal(temp_c);
    
        const DataPacket temp_packet = create_temp_packet((uint16_t) temp_int);
        uart_mgr_transmit(&temp_packet);
        
        char whole[8];
        itoa(whole, temp_int / 100);


        char fraction[8];
        itoa(fraction, temp_int % 100);
        
        lcd_mgr_clear();
        _delay_ms(2);
        lcd_mgr_write("Temp: ");
        lcd_mgr_write(whole);
        lcd_mgr_write(".");
        lcd_mgr_write(fraction);

        ticks = 0;
    }
}

static int16_t remove_decimal(float x) {
    if (x >= 0.0f) {
        return 100.0f * x + 0.5f;
    } else {
        return 100.0f * x - 0.5f;
    }
}

static DataPacket create_temp_packet(uint16_t temp) {
    DataPacket temp_packet;
    temp_packet.start_byte = START_BYTE;
    temp_packet.type = TEMP;
    temp_packet.length = 2;
    
    uint8_t high_byte = (temp >> 8);
    uint8_t low_byte = temp & 0xFF;

    temp_packet.payload[0] = high_byte;
    temp_packet.payload[1] = low_byte;
    
    temp_packet.checksum = 2;
    return temp_packet;
}

static void itoa(char *string, int16_t value) {
    if (value < 0) {
        *string++ = '-';
    }

    uint16_t pos_val = (value < 0) ? -value : value;
    
    if (pos_val == 0) {
        *string++ = '0';
    } else {
        char interim[5] = {0};
        int i = 0;
        while (pos_val > 0) {
            // Get digits as chars
            interim[i++] = '0' + (pos_val % 10);
            pos_val /= 10;
        }
        while (i > 0) {
            // Reverse to get original int
            *string++ = interim[--i];
        }

    }

    *string = '\0';
}
/*
static void itoa(char *string, int16_t value) {
    if (value == 0) {
        *string++ = '0';
    } else {
        uint16_t pos_val = (value < 0) ? -value : value;
        if (value < 0) {
            *string++ = '-';
        }

        char interim[5] = {0};
        int i = 0;
        while (pos_val > 0) {
            interim[i++] = '0' + (pos_val % 10);
            pos_val /= 10;
        }

        while (i > 0) {
            *string++ = interim[--i];
        }
    }

    *string = '\0';  // Ensure null termination.
}
*/
