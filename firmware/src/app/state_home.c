#include <stdint.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include <thermostat/protocol.h>
#include "states.h"

#define TEMP_DELAY  125000UL
#define GO_STATS    '#'

static struct {
    uint32_t timer;
    int16_t temp;
    char key;
    struct data_packet temp_packet;
    struct {
        uint8_t lcd_dirty : 1;
        uint8_t tx_pending : 1;
        uint8_t key_pending : 1;
        uint8_t stats_pending : 1;
    } flags;
} home_data;

static void home_init(void);
static void home_keypress(void);
static void home_process(void);
static void home_display(void);
static void home_send(void);

const struct state_actions home_state = {
    .init        = home_init,
    .on_keypress = home_keypress,
    .display     = home_display,
    .process     = home_process,
    .send        = home_send,
    .receive     = 0,
    .exit        = 0
};

static int16_t format_temp(float temp);
static void configure_temp_packet(int16_t temp_int, struct data_packet *pkt_out);

static void home_init(void) {
    home_data.timer = TEMP_DELAY;
    home_data.flags.lcd_dirty = 0;
    home_data.flags.tx_pending = 0;
    home_data.flags.key_pending = 0;
    home_data.flags.stats_pending = 0;
}

static void home_keypress(void) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key)
        return;

    home_data.flags.key_pending = 1;
    home_data.key = keypad.current_key;
}

static void home_process(void) {
    if (home_data.flags.key_pending) {
        home_data.flags.key_pending = 0;
        switch (home_data.key) {
        case GO_STATS:
            sys_change_state(&stats_state);
            return;
        default:
            break;
    }

    ++home_data.timer;
    if (home_data.timer < TEMP_DELAY)
        return;
    
    home_data.timer = 0;
    const float raw_temp = therm_mgr_get_temp();
    home_data.temp = format_temp(raw_temp);
    home_data.flags.lcd_dirty = 1;
    home_data.flags.tx_pending = 1;
}

static void home_display(void) {
    if (!home_data.flags.lcd_dirty)
        return;

    home_data.flags.lcd_dirty = 0;
    lcd_mgr_clear();
    lcd_mgr_write("Temp: ");
    lcd_mgr_write_int(home_data.temp / 100); // High part
    lcd_mgr_write(".");
    lcd_mgr_write_int(home_data.temp % 100); // Low part
}

static void home_send(void) {
    if (!home_data.flags.tx_pending)
        return;

    home_data.flags.tx_pending = 0;
    configure_temp_packet(home_data.temp, &home_data.temp_packet);
    uart_mgr_transmit(&home_data.temp_packet);
}

static int16_t format_temp(float temp) {
    if (temp > 0.0f) {
        return (int16_t) 100.0f * temp + 0.5f;
    } else {
        return (int16_t) 100.0f * temp - 0.5f;
    }
}

static void configure_temp_packet(int16_t temp_int, struct data_packet *pkt_out) {
    pkt_out->start_byte = START_BYTE;
    pkt_out->type = TEMP; pkt_out->length = 2;

    uint8_t high_byte = (uint8_t) (temp_int >> 8);
    uint8_t low_byte = (uint8_t) (temp_int & 0xFF);
    pkt_out->payload[0] = high_byte;
    pkt_out->payload[1] = low_byte;

    pkt_out->checksum = 2;
}
