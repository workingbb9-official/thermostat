#include "app/system_core.h"

#include <stdint.h>

#include "thermistor/thermistor.h"
#include "uart/uart.h"
#include "lcd/lcd.h"
#include "keypad/keypad.h"
#include "common/protocol.h"
#include "app/states.h"
#include "app/state_login.h"

#define TEMP_DELAY 1250000UL

static enum sys_state current_state = STATE_LOGIN;

static void home_loop(void);
static int16_t get_temp(void);
static int16_t remove_decimal(int x);
static void send_temp(int16_t temp);

void system_init(void) {
    therm_mgr_init();
    uart_mgr_init();
    lcd_mgr_init();
    keypad_init();
}

void system_run(void) {
    switch (current_state) {
    case STATE_LOGIN:
        login_run(&current_state);
        break;
    case STATE_HOME:
        home_loop();
        break;
    default:
        current_state = STATE_LOGIN;
        break;
    }
}

static void home_loop(void) {
    static volatile uint32_t ticks = 0;
    ++ticks;

    if (ticks >= TEMP_DELAY) {
        const int16_t temp_int = get_temp();
        send_temp(temp_int);

        int16_t whole = temp_int / 100;
        int16_t fraction = temp_int % 100;
       
        lcd_mgr_clear();
        lcd_mgr_write("Temp: ");
        if (temp_int < 0) {
            lcd_mgr_write("-");
        }
        lcd_mgr_write_int(whole);
        lcd_mgr_write(".");
        lcd_mgr_write_int(fraction);

        ticks = 0;
    }
}

static int16_t get_temp(void) {
    const float temp_c = therm_mgr_get_temp();
    const int16_t temp_int = remove_decimal(temp_c);
    return temp_int;
}

static int16_t remove_decimal(int x) {
    if (x >= 0.0f) {
        return (int16_t) 100.0f * x + 0.5f;
    } else {
        return (int16_t) 100.0f * x - 0.5f;
    }
}

static void send_temp(int16_t temp) {
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
