#include <stdint.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>
#include "states.h"

#define STATS_DELAY 700000UL
#define GO_HOME 'A'
#define SWITCH_EXTREME 'B'
#define FALSE 0
#define TRUE (!FALSE)

enum extreme {
    MAX = 0,
    MIN
} __attribute__((packed));

struct stats {
   int16_t avg;
   int16_t max;
   int16_t min;
}; 

static struct {
    uint32_t timer;
    enum extreme current_extreme;
    struct stats current_stats;
    uint8_t display_stats : 1;
    uint8_t receive_stats : 1;
    uint8_t request_stats : 1;
    uint8_t go_home : 1;
} stats_data;

void stats_init(void);
void stats_keypress(void);
void stats_process(void);
void stats_display(void);
void stats_send(void);
void stats_receive(void);
void stats_exit(void);

const struct state_actions stats_state = {
    .init = &stats_init,
    .on_keypress = &stats_keypress,
    .process = &stats_process,
    .display = &stats_display,
    .send = &stats_send,
    .receive = &stats_receive,
    .exit = 0
};

static void receive_stats(struct stats *stats_out);
static void display_stats(const struct stats *current_stats, enum extreme current_extreme);

void stats_init(void) {
    stats_data.timer = STATS_DELAY;
    stats_data.current_extreme = MAX;
}

void stats_keypress(void) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key) {
        return;
    }

    if (keypad.current_key == GO_HOME) {
        stats_data.go_home = TRUE;
    } else if (keypad.current_key == SWITCH_EXTREME) {
        stats_data.current_extreme = !stats_data.current_extreme;
        stats_data.display_stats = TRUE;
    }
}

void stats_process(void) {
    if (stats_data.go_home) {
        sys_change_state(&home_state);
        stats_data.go_home = FALSE;
        return;
    }
    
    ++stats_data.timer;
    if (stats_data.timer >= STATS_DELAY) {
        stats_data.request_stats = TRUE;
        stats_data.timer = 0;
    }
}

void stats_display(void) {
    if (stats_data.display_stats) {
        display_stats(&stats_data.current_stats, stats_data.current_extreme);
        stats_data.display_stats = FALSE;
    }
}

void stats_send(void) {
    if (stats_data.request_stats) {
        struct data_packet stats_req = {
            .start_byte = START_BYTE,
            .type = STATS,
            .length = 1,
            .payload[0] = START_BYTE,
            .checksum = 1
        };
        uart_mgr_transmit(&stats_req);
        stats_data.request_stats = FALSE;
        stats_data.receive_stats = TRUE;
    }
}

void stats_receive(void) {
    if (stats_data.receive_stats) {
        receive_stats(&stats_data.current_stats);
        stats_data.receive_stats = FALSE;
        stats_data.display_stats = TRUE;
    }
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
