#include <stdint.h>
#include <avr/pgmspace.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include <thermostat/protocol.h>
#include "states.h"

#define HOME_DELAY_TICKS 125000UL
#define HOME_KEY_STATS   '#'

static struct {
    uint32_t ticks;
    char input;

    struct {
        int16_t value;
        struct data_packet packet;
    } temp;

    union {
        uint8_t all;
        struct {
            uint8_t lcd_dirty       : 1;
            uint8_t tx_req          : 1;

            uint8_t input_pending   : 1;
            uint8_t reserved        : 4;
        }; 
    } flags;
} home_ctx;

static void home_init(void);
static void home_keypress(void);
static void home_process(void);
static void home_display(void);
static void home_send(void);

const struct state_ops state_home = {
    .init           = home_init,
    .on_keypress    = home_keypress,
    .display        = home_display,
    .process        = home_process,
    .send           = home_send,
    .receive        = NULL,
    .exit           = NULL
};

static int16_t format_temp(float temp);
static void configure_temp_packet(void);

static void home_init(void) {
    home_ctx.ticks = HOME_DELAY_TICKS;
    home_ctx.flags.all = 0;
}

static void home_keypress(void) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key)
        return;
    
    home_ctx.flags.input_pending = 1;
    home_ctx.input = keypad.current_key;
}

static void home_process(void) {
    if (home_ctx.flags.input_pending) {
        home_ctx.flags.input_pending = 0;

        switch (home_ctx.input) {
        case HOME_KEY_STATS:
            sys_change_state(&state_stats);
            return;
        default:
            break;
        }
    }

    if (++home_ctx.ticks < HOME_DELAY_TICKS)
        return;

    home_ctx.ticks = 0;

    const float raw_temp = therm_mgr_get_temp();
    home_ctx.temp.value = format_temp(raw_temp);

    home_ctx.flags.lcd_dirty = 1;
    home_ctx.flags.tx_req = 1;
}

static void home_display(void) {
    if (!home_ctx.flags.lcd_dirty)
        return;

    home_ctx.flags.lcd_dirty = 0;

    lcd_mgr_clear();
    lcd_mgr_write_p(PSTR("Temp: "));
    lcd_mgr_write_int(home_ctx.temp.value / 100);
    lcd_mgr_write(".");
    lcd_mgr_write_int(home_ctx.temp.value % 100);
}

static void home_send(void) {
    if (!home_ctx.flags.tx_req)
        return;

    home_ctx.flags.tx_req = 0;

    configure_temp_packet();
    uart_mgr_transmit(&home_ctx.temp.packet);
}

static int16_t format_temp(float temp) {
    if (temp > 0.0f) {
        return (int16_t) 100.0f * temp + 0.5f;
    } else {
        return (int16_t) 100.0f * temp - 0.5f;
    }
}

static void configure_temp_packet(void) {
    struct data_packet *packet = &home_ctx.temp.packet;

    packet->start_byte = START_BYTE;
    packet->type = HOME; 
    packet->length = 2;

    packet->payload[0] = (uint8_t) (home_ctx.temp.value >> 8);
    packet->payload[1] = (uint8_t) (home_ctx.temp.value & 0xFF);

    packet->checksum = 2;
}
