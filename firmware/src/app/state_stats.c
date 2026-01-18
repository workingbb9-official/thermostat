#include <stdint.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>
#include "states.h"

#define STATS_DELAY_TICKS       700000UL
#define STATS_KEY_HOME          'A'
#define STATS_KEY_SWITCH_LIMIT  'B'

static struct {
    uint32_t ticks;
    char input;
    
    enum {
        MAX = 0,
        MIN
    } limit __attribute__((packed));

    struct {
        int16_t avg;
        int16_t max;
        int16_t min;
    } stats;
    
    union {
        uint8_t all;

        struct {
            uint8_t lcd_dirty       : 1;
            uint8_t tx_req          : 1;
            uint8_t rx_req          : 1;

            uint8_t input_pending   : 1;
            uint8_t tx_complete     : 1;
            uint8_t rx_complete     : 1;

            uint8_t reserved        : 2;
        };
    } flags;
} stats_ctx;

static void stats_init(void);
static void stats_keypress(void);
static void stats_process(void);
static void stats_display(void);
static void stats_send(void);
static void stats_receive(void);

const struct state_actions stats_state = {
    .init           = stats_init,
    .on_keypress    = stats_keypress,
    .process        = stats_process,
    .display        = stats_display,
    .send           = stats_send,
    .receive        = stats_receive,
    .exit           = 0
};

static void stats_init(void) {
    stats_ctx.ticks = STATS_DELAY_TICKS;
    stats_ctx.limit = MAX;
    stats_ctx.flags.all = 0;
}

static void stats_keypress(void) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key)
        return;
    
    stats_ctx.flags.input_pending = 1;
    stats_ctx.input = keypad.current_key;
}

static void stats_process(void) {
    if (stats_ctx.flags.input_pending) {
        stats_ctx.flags.input_pending = 0;

        switch (stats_ctx.input) {
        case STATS_KEY_HOME:
            sys_change_state(&home_state);
            return;
        case STATS_KEY_SWITCH_LIMIT:
            stats_ctx.limit = !stats_ctx.limit;
            stats_ctx.flags.lcd_dirty = 1;
            break;
        default:
            break;
        }
    }
    
    if (stats_ctx.flags.tx_complete) {
        stats_ctx.flags.tx_complete = 0;
        stats_ctx.flags.rx_req = 1;
    }

    if (stats_ctx.flags.rx_complete) {
        stats_ctx.flags.rx_complete = 0;
        stats_ctx.flags.lcd_dirty = 1;
    }
    
    if (++stats_ctx.ticks < STATS_DELAY_TICKS)
        return;

    stats_ctx.ticks = 0;

    stats_ctx.flags.tx_req = 1;
}

static void stats_display(void) {
    if (!stats_ctx.flags.lcd_dirty)
        return;

    stats_ctx.flags.lcd_dirty = 0;

    lcd_mgr_clear();
    lcd_mgr_write("Average: ");
    lcd_mgr_write_int(stats_ctx.stats.avg / 100);
    lcd_mgr_write(".");
    lcd_mgr_write_int(stats_ctx.stats.avg % 100);
    lcd_mgr_set_cursor(1, 0);

    if (stats_ctx.limit == MAX) {
        lcd_mgr_write("Max: ");
        lcd_mgr_write_int(stats_ctx.stats.max / 100);
        lcd_mgr_write(".");
        lcd_mgr_write_int(stats_ctx.stats.max % 100);
    } else {
        lcd_mgr_write("Min: ");
        lcd_mgr_write_int(stats_ctx.stats.min / 100);
        lcd_mgr_write(".");
        lcd_mgr_write_int(stats_ctx.stats.min % 100);
    }
}

static void stats_send(void) {
    if (!stats_ctx.flags.tx_req)
        return;

    stats_ctx.flags.tx_req = 0;

    struct data_packet stats_req = {
        .start_byte = START_BYTE,
        .type       = STATS,
        .length     = 1,
        .payload[0] = 0x01,
        .checksum   = 1
    };

    uart_mgr_transmit(&stats_req);
    stats_ctx.flags.tx_complete = 1;
}

static void stats_receive(void) {
    if (!stats_ctx.flags.rx_req)
        return;
    
    stats_ctx.flags.rx_req = 0;

    struct data_packet stats_packet;
    if (uart_mgr_receive(&stats_packet) != VALID_PACKET)
        return;

    stats_ctx.stats.avg = (int16_t) (((uint16_t) stats_packet.payload[0] << 8) | stats_packet.payload[1]);
    stats_ctx.stats.max = (int16_t) (((uint16_t) stats_packet.payload[2] << 8) | stats_packet.payload[3]);
    stats_ctx.stats.min = (int16_t) (((uint16_t) stats_packet.payload[4] << 8) | stats_packet.payload[5]);

    stats_ctx.flags.rx_complete = 1;
}
