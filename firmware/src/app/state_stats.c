#include "state_stats.h"

#include <stdint.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>
#include "states.h"

#define STATS_DELAY 1000000UL
#define GO_HOME 'A'
#define SWITCH_EXTREME 'B'

enum extreme {
    MAX = 0,
    MIN
} __attribute__((packed));

static void ask_for_stats(void);
static void receive_and_display_stats(enum extreme current_extreme);

void stats_run(enum sys_state *current_state) {
    static volatile uint32_t stats_timer = (uint32_t) STATS_DELAY;
    static enum extreme current_extreme = MAX;
    ++stats_timer;

    if (stats_timer >= STATS_DELAY) {
        ask_for_stats();
        receive_and_display_stats(current_extreme);
        stats_timer = 0;
    }

    const char key = keypad_read();
    if (key == GO_HOME) {
        stats_timer = (uint32_t) STATS_DELAY;
        *current_state = STATE_HOME;
    } else if (key == SWITCH_EXTREME) {
        current_extreme = !current_extreme;
        stats_timer = (uint32_t) STATS_DELAY;
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

static void receive_and_display_stats(enum extreme current_extreme) {
    struct data_packet stats_packet = {0};
    if (uart_mgr_receive(&stats_packet) != VALID_PACKET) {
        lcd_mgr_clear();
        lcd_mgr_write("Invalid packet");
        return;
    }

    const int16_t average = (int16_t) (((uint16_t) stats_packet.payload[0] << 8) | stats_packet.payload[1]);
    lcd_mgr_clear();
    lcd_mgr_write("Average: ");
    lcd_mgr_write_int(average / 100);
    lcd_mgr_write(".");
    lcd_mgr_write_int(average % 100);
    
    if (current_extreme == MAX) {
        const int16_t max = (int16_t) (((uint16_t) stats_packet.payload[2] << 8) | stats_packet.payload[3]);
        lcd_mgr_set_cursor(1, 0);
        lcd_mgr_write("Max: ");
        lcd_mgr_write_int(max / 100);
        lcd_mgr_write(".");
        lcd_mgr_write_int(max % 100);
    } else {
        const int16_t min = (int16_t) (((uint16_t) stats_packet.payload[4] << 8) | stats_packet.payload[5]);
        lcd_mgr_set_cursor(1, 0);
        lcd_mgr_write("Min: ");
        lcd_mgr_write_int(min / 100);
        lcd_mgr_write(".");
        lcd_mgr_write_int(min % 100);
    }
}
