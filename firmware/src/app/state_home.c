#include <stdint.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include <thermostat/protocol.h>
#include "states.h"

#define TEMP_DELAY 125000UL
#define GO_STATS '#'
#define FALSE 0
#define TRUE (!FALSE)

static struct {
    uint32_t timer;
    int16_t temp;
    uint8_t display_temp : 1;
    uint8_t send_temp : 1;
} home_data = {
    .timer = 0,
    .temp = 0
};

static void home_init(void);
static void home_keypress(void);
static void home_process(void);
static void home_display(void);
static void home_send(void);
static void home_exit(void);

const struct state_actions home_state = {
    .init = &home_init,
    .on_keypress = &home_keypress,
    .display = &home_display,
    .process = &home_process,
    .send = &home_send,
    .receive = 0,
    .exit = &home_exit
};

static int16_t format_temp(float temp);
static struct data_packet create_temp_packet(int16_t temp_int);

static void home_init(void) {
    home_data.timer = TEMP_DELAY;
}

static void home_keypress(void) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == GO_STATS) {
        sys_change_state(&stats_state);
    }
}

static void home_process(void) {
    ++home_data.timer;
    if (home_data.timer >= TEMP_DELAY) {
        const float raw_temp = therm_mgr_get_temp();
        home_data.temp = format_temp(raw_temp);
        home_data.display_temp = TRUE;
        home_data.send_temp = TRUE;
        home_data.timer = 0;
    }
}

static void home_display(void) {
    if (home_data.display_temp) {
        lcd_mgr_clear();
        lcd_mgr_write("Temp: ");
        lcd_mgr_write_int(home_data.temp / 100); // High part
        lcd_mgr_write(".");
        lcd_mgr_write_int(home_data.temp % 100); // Low part
        home_data.display_temp = FALSE;
    }
}

static void home_send(void) {
    if (home_data.send_temp) {
        struct data_packet temp_packet = create_temp_packet(home_data.temp);
        uart_mgr_transmit(&temp_packet);
        home_data.send_temp = FALSE;
    }
}

static void home_exit(void) {
    home_data.timer = TEMP_DELAY;
}

static int16_t format_temp(float temp) {
    if (temp > 0.0f) {
        return (int16_t) 100.0f * temp + 0.5f;
    } else {
        return (int16_t) 100.0f * temp - 0.5f;
    }
}

static struct data_packet create_temp_packet(int16_t temp_int) {
    struct data_packet temp_packet;
    temp_packet.start_byte = START_BYTE;
    temp_packet.type = TEMP; temp_packet.length = 2;

    uint8_t high_byte = (uint8_t) (temp_int >> 8);
    uint8_t low_byte = (uint8_t) (temp_int & 0xFF);
    temp_packet.payload[0] = high_byte;
    temp_packet.payload[1] = low_byte;

    temp_packet.checksum = 2;
    return temp_packet;
}
