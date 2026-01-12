#include "state_stats.h"

#include <stdint.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>
#include "states.h"

#define STATS_DELAY 100000UL
#define WRITE_STATS_SCREEN 0
#define WRITE_NOT_COMPLETE 1

static void ask_for_stats(void);
static void receive_and_display_stats(void);

void stats_run(enum sys_state *current_state) {
    static volatile uint32_t stats_timer = (uint32_t) STATS_DELAY;
    ++stats_timer;

    if (stats_timer >= STATS_DELAY) {
        ask_for_stats();
        receive_and_display_stats();
        stats_timer = 0;
    }

    const char key = keypad_read();
    if (key == 'A') {
        stats_timer = (uint32_t) STATS_DELAY;
        *current_state = STATE_HOME;
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

static void receive_and_display_stats(void) {
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
}
