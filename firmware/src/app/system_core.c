#include "app/system_core.h"

#include <stdint.h>

#include "app/system_temperature.h"
#include "uart/uart.h"
#include "lcd/lcd.h"
#include "keypad/keypad.h"
#include "common/protocol.h"

#define TEMP_DELAY 1250000

static SysState current_state = STATE_LOGIN;
const char real_password[PASSWORD_LEN] = {'1', '2', '3', '4'};

static void login_loop(void);
static int8_t check_password(const char *password);

static void home_loop(void);
static void send_temp(int16_t temp);

void system_init(void) {
    keypad_init();
    system_temperature_init();
    uart_mgr_init();
    lcd_mgr_init();
}

void system_run(void) {
    switch (current_state) {
    case STATE_LOGIN:
        login_loop();
        break;
    case STATE_HOME:
        home_loop();
        break;
    default:
        current_state = STATE_LOGIN;
        break;
    }
}

static void login_loop(void) {
    static uint8_t password_pos = 0;
    static char password[PASSWORD_LEN]; 
    static char last_key;

    const char key = keypad_read();
    if (key == NO_KEY) {
        last_key = NO_KEY;
        return;
    } else if (key == last_key) {
        return;
    }
    
    password[password_pos] = key;
    password[password_pos + 1] = '\0';
    last_key = key;
    ++password_pos;

    lcd_mgr_clear();
    lcd_mgr_write(password);

    if (password_pos >= PASSWORD_LEN) {
        if (check_password(password) == CORRECT) {
            current_state = STATE_HOME;
            lcd_mgr_clear();
            lcd_mgr_write("Validated");
        } else {
            lcd_mgr_clear();
            lcd_mgr_write("Invalid");
        }
        
        password_pos = 0;
        password[0] = '\0';
    }
}

static int8_t check_password(const char *password) {
    for (uint8_t i = 0; i < PASSWORD_LEN; ++i) {
        if (password[i] != real_password[i]) {
            return WRONG;
        }
    }

    return CORRECT;
}

static void home_loop(void) {
    static volatile uint32_t ticks = 0;
    ++ticks;

    if (ticks >= TEMP_DELAY) {
        const int16_t temp_int = system_get_temp();
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
