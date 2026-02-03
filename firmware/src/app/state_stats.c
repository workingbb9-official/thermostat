#include <stdint.h>
#include <avr/pgmspace.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>
#include "states.h"
#include "strings.h"

#define STATS_DELAY_TICKS      700000UL
#define STATS_KEY_HOME         'A'
#define STATS_KEY_SWITCH_LIMIT 'B'

static struct {
    uint32_t ticks;
    char input;

    enum { 
        MAX = 0,
        MIN
    } __attribute__((packed)) limit; 

    struct {
        int16_t avg;
        int16_t max;
        int16_t min;
    } stats;

    struct rx_ctx rx;
    struct data_packet rx_pkt;
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

const struct state_ops state_stats = {
    .init     = stats_init,
    .keypress = stats_keypress,
    .process  = stats_process,
    .display  = stats_display,
    .send     = stats_send,
    .receive  = stats_receive,
    .exit     = NULL
};

static void stats_init(void) {
    stats_ctx.ticks = STATS_DELAY_TICKS - 20;
    stats_ctx.limit = MAX;

    stats_ctx.rx.stage = 0;
    stats_ctx.rx.payload_idx = 0;
    stats_ctx.flags.all = 0;
    stats_ctx.flags.lcd_dirty = 1;

    uart_clear_rx();
}

static void stats_keypress(void) {
    const struct keypad_state keypad = keypad_get_state();
    if (keypad.current_key == KEYPAD_NO_KEY ||
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
            sys_change_state(&state_home);
            return;

        case STATS_KEY_SWITCH_LIMIT:
            stats_ctx.limit = !stats_ctx.limit;
            stats_ctx.flags.lcd_dirty = 1;
            break;

        default:
            break;
        }
    }
    
    // After transmit, expect return
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

    stats_ctx.flags.tx_req = 1; // Send request for stats
}

static void stats_display(void) {
    if (!stats_ctx.flags.lcd_dirty)
        return;

    stats_ctx.flags.lcd_dirty = 0;
    
    // Display average
    lcd_clear();
    lcd_draw_pstr(PSTR("Average: "));
    lcd_draw_int(stats_ctx.stats.avg / 100);
    lcd_draw_pstr(dot);

    int16_t avg_decimal = stats_ctx.stats.avg % 100;
    if (avg_decimal < 0)
        avg_decimal = -avg_decimal;

    lcd_draw_int(stats_ctx.stats.avg % 100);
    lcd_set_cursor(1, 0);

    if (stats_ctx.limit == MAX) {
        // Display max
        lcd_draw_pstr(PSTR("Max: "));
        lcd_draw_int(stats_ctx.stats.max / 100);
        lcd_draw_pstr(dot);

        int16_t max_decimal = stats_ctx.stats.max % 100;
        if (max_decimal < 0)
            max_decimal = -max_decimal;

        lcd_draw_int(stats_ctx.stats.max % 100);
    } else {
        // Display min
        lcd_draw_pstr(PSTR("Min: "));
        lcd_draw_int(stats_ctx.stats.min / 100);
        lcd_draw_pstr(dot);

        int16_t min_decimal = stats_ctx.stats.min % 100;
        if (min_decimal < 0)
            min_decimal = -min_decimal;

        lcd_draw_int(stats_ctx.stats.min % 100);
    }
}

static void stats_send(void) {
    if (!stats_ctx.flags.tx_req)
        return;

    stats_ctx.flags.tx_req = 0;
    stats_ctx.flags.tx_complete = 1;

    struct data_packet stats_req = {
        .start_byte = START_BYTE,
        .type       = STATS,
        .length     = 1,
        .payload[0] = PAYLOAD_NONE,
        .checksum   = 1
    };

    uart_send_packet(&stats_req);
}

static void stats_receive(void) {
    if (!stats_ctx.flags.rx_req)
        return;

    struct data_packet *pkt = &stats_ctx.rx_pkt;
    int8_t rx_status = uart_receive_packet(&stats_ctx.rx, pkt);

    // Check for errors
    if (rx_status < 0 && rx_status != UART_INCOMPLETE) {
        stats_ctx.rx.stage = 0;
        stats_ctx.rx.payload_idx = 0;
        return;
    }

    // If incomplete, try again next loop
    if (rx_status == UART_INCOMPLETE)
        return;

    // Store stats
    stats_ctx.stats.avg = 
        (int16_t) (((uint16_t) pkt->payload[0] << 8) | pkt->payload[1]);
    stats_ctx.stats.max = 
        (int16_t) (((uint16_t) pkt->payload[2] << 8) | pkt->payload[3]);
    stats_ctx.stats.min = 
        (int16_t) (((uint16_t) pkt->payload[4] << 8) | pkt->payload[5]);
    
    // Reset for the next packet
    stats_ctx.rx.stage = 0;
    stats_ctx.rx.payload_idx = 0;

    stats_ctx.flags.rx_req = 0;
    stats_ctx.flags.rx_complete = 1;
}
