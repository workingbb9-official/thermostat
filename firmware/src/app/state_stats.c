#include <stdint.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>
#include "states.h"

#define STATS_DELAY     700000UL
#define GO_HOME         'A'
#define SWITCH_LIMIT    'B'

enum limit_type {
    LIMIT_MAX = 0,
    LIMIT_MIN
} __attribute__((packed));

static struct {
    uint32_t timer;
    enum limit_type limit_mode;
    char key;
    
    struct {
        int16_t avg;
        int16_t max;
        int16_t min;
    } stats;

    struct {
        uint8_t lcd_dirty    : 1;
        uint8_t rx_complete  : 1;
        uint8_t tx_complete  : 1;
        uint8_t new_key      : 1;

        uint8_t rx_pending   : 1;
        uint8_t tx_pending   : 1;
        uint8_t key_pending  : 1;
        uint8_t home_pending : 1;
    } flags;
} stats_data;

static void stats_init(void);
static void stats_keypress(void);
static void stats_process(void);
static void stats_display(void);
static void stats_send(void);
static void stats_receive(void);

const struct state_actions stats_state = {
    .init        = stats_init,
    .on_keypress = stats_keypress,
    .process     = stats_process,
    .display     = stats_display,
    .send        = stats_send,
    .receive     = stats_receive,
    .exit        = 0
};

static void stats_init(void) {
    stats_data.timer = STATS_DELAY;
    stats_data.limit_mode = LIMIT_MAX;

    stats_data.flags.new_key = 0;
    stats_data.flags.rx_pending = 0;
    stats_data.flags.tx_pending = 0;
    stats_data.flags.home_pending = 0;

    stats_data.stats.avg = 0;
    stats_data.stats.max = 0;
    stats_data.stats.min = 0;
}

static void stats_keypress(void) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key)
        return;
    
    stats_data.flags.new_key = 1;
    stats_data.key = keypad.current_key;
}

static void stats_process(void) {
    if (stats_data.flags.new_key) {
        stats_data.flags.new_key = 0;
        switch (stats_data.key) {
        case GO_HOME:
            sys_change_state(&home_state);
            return;
        case SWITCH_LIMIT:
            stats_data.limit_mode = !stats_data.limit_mode;
            stats_data.flags.lcd_dirty = 1;
            break;
        default:
            break;
        }
    }
    
    if (stats_data.flags.tx_complete) {
        stats_data.flags.tx_complete = 0;
        stats_data.flags.rx_pending = 1;
    }

    if (stats_data.flags.rx_complete) {
        stats_data.flags.rx_complete = 0;
        stats_data.flags.lcd_dirty = 1;
    }

    if (++stats_data.timer >= STATS_DELAY) {
        stats_data.timer = 0;
        stats_data.flags.tx_pending = 1;
    }
}

static void stats_display(void) {
    if (!stats_data.flags.lcd_dirty)
        return;

    stats_data.flags.lcd_dirty = 0;
    lcd_mgr_clear();

    lcd_mgr_write("Average: ");
    lcd_mgr_write_int(stats_data.stats.avg / 100);
    lcd_mgr_write(".");
    lcd_mgr_write_int(stats_data.stats.avg % 100);
    lcd_mgr_set_cursor(1, 0);

    if (stats_data.limit_mode == LIMIT_MAX) {
        lcd_mgr_write("Max: ");
        lcd_mgr_write_int(stats_data.stats.max / 100);
        lcd_mgr_write(".");
        lcd_mgr_write_int(stats_data.stats.max % 100);
    } else {
        lcd_mgr_write("Min: ");
        lcd_mgr_write_int(stats_data.stats.min / 100);
        lcd_mgr_write(".");
        lcd_mgr_write_int(stats_data.stats.min % 100);
    }
}

static void stats_send(void) {
    if (!stats_data.flags.tx_pending)
        return;

    stats_data.flags.tx_pending = 0;

    struct data_packet stats_req = {
        .start_byte = START_BYTE,
        .type       = STATS,
        .length     = 1,
        .payload[0] = 0x01,
        .checksum   = 1
    };
    uart_mgr_transmit(&stats_req);

    stats_data.flags.tx_complete = 1;
}

static void stats_receive(void) {
    if (!stats_data.flags.rx_pending)
        return;
    
    stats_data.flags.rx_pending = 0;

    struct data_packet stats_packet;
    if (uart_mgr_receive(&stats_packet) != VALID_PACKET)
        return;

    stats_data.stats.avg = (int16_t) (((uint16_t) stats_packet.payload[0] << 8) | stats_packet.payload[1]);
    stats_data.stats.max = (int16_t) (((uint16_t) stats_packet.payload[2] << 8) | stats_packet.payload[3]);
    stats_data.stats.min = (int16_t) (((uint16_t) stats_packet.payload[4] << 8) | stats_packet.payload[5]);

    stats_data.flags.rx_complete = 1;
}
