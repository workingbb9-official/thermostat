#include "state_stats.h"

#include <stdint.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include "states.h"

#define STATS_DELAY 100000UL
#define WRITE_STATS_SCREEN 0
#define WRITE_NOT_COMPLETE 1

static void display_text(uint8_t text_to_write);

void stats_run(enum sys_state *current_state) {
    static volatile uint32_t stats_timer = (uint32_t) STATS_DELAY;
    ++stats_timer;
    
    static uint8_t text_to_write = WRITE_STATS_SCREEN;
    if (stats_timer >= STATS_DELAY) {
        display_text(text_to_write);
        text_to_write = !text_to_write;
        stats_timer = 0;
    }

    const char key = keypad_read();
    if (key == 'A') {
        stats_timer = (uint32_t) STATS_DELAY;
        text_to_write = WRITE_STATS_SCREEN;
        *current_state = STATE_HOME;
    }
}

static void display_text(uint8_t text_to_write) {
    if (text_to_write == WRITE_STATS_SCREEN) {
        lcd_mgr_clear();
        lcd_mgr_write("Stats screen");
    } else {
        lcd_mgr_clear();
        lcd_mgr_write("Not complete");
    }
}
