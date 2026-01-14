#include "state_stats.h"

#include <stdint.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>
#include "states.h"

#define STATS_DELAY 700000UL
#define GO_HOME 'A'
#define SWITCH_EXTREME 'B'

enum extreme {
    MAX = 0,
    MIN
} __attribute__((packed));

struct stats {
   int16_t avg;
   int16_t max;
   int16_t min;
} __attribute__((packed));

static void ask_for_stats(void);
static void receive_stats(struct stats *stats_out);
static void display_stats(const struct stats *current_stats, enum extreme current_extreme);

void stats_run(enum sys_state *current_state) {
    static struct stats current_stats= {0};
    static enum extreme current_extreme = MAX;
    static volatile uint32_t stats_timer = (uint32_t) STATS_DELAY;
    ++stats_timer;

    if (stats_timer >= STATS_DELAY) {
        ask_for_stats();
        receive_stats(&current_stats);
        display_stats(&current_stats, current_extreme);
        stats_timer = 0;
    }

    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key) {
        return;
    }

    if (keypad.current_key == GO_HOME) {
        stats_timer = (uint32_t) STATS_DELAY;
        *current_state = STATE_HOME;
    } else if (keypad.current_key == SWITCH_EXTREME) {
        current_extreme = !current_extreme;
        display_stats(&current_stats, current_extreme);
    }
}

static void ask_for_stats(void) {
    struct data_packet request_packet = {0};
    request_packet.start_byte = START_BYTE;
    request_packet.type = STATS;
    request_packet.length = 1;
    request_packet.payload[0] = START_BYTE;
    request_packet.checksum = 1;
    uart_mgr_transmit(&request_packet);
}

static void receive_stats(struct stats *stats_out) {
    struct data_packet stats_packet = {0};
    if (uart_mgr_receive(&stats_packet) != VALID_PACKET) {
        lcd_mgr_clear();
        lcd_mgr_write("Invalid packet");
        return;
    }

    stats_out->avg = (int16_t) (((uint16_t) stats_packet.payload[0] << 8) | stats_packet.payload[1]);
    stats_out->max = (int16_t) (((uint16_t) stats_packet.payload[2] << 8) | stats_packet.payload[3]);
    stats_out->min = (int16_t) (((uint16_t) stats_packet.payload[4] << 8) | stats_packet.payload[5]);
}

static void display_stats(const struct stats *current_stats, enum extreme current_extreme) {
    lcd_mgr_clear();
    lcd_mgr_write("Average: ");
    lcd_mgr_write_int(current_stats->avg / 100);
    lcd_mgr_write(".");
    lcd_mgr_write_int(current_stats->avg % 100);

    lcd_mgr_set_cursor(1, 0);
    if (current_extreme == MAX) {
        lcd_mgr_write("Max: ");
        lcd_mgr_write_int(current_stats->max / 100);
        lcd_mgr_write(".");
        lcd_mgr_write_int(current_stats->max % 100);
    } else {
        lcd_mgr_write("Min: ");
        lcd_mgr_write_int(current_stats->min / 100);
        lcd_mgr_write(".");
        lcd_mgr_write_int(current_stats->min % 100);
    }
}
