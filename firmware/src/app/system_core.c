#include "app/system_core.h"

#include <stdint.h>

#include "app/system_temperature.h"
#include "app/system_data_handler.h"
#include "app/system_display.h"

#define TEMP_DELAY 1000000

void system_init(void) {
    system_temperature_init();
    system_data_handler_init();
    system_display_init();
}

void system_run(void) {
    static volatile uint32_t ticks = 0;
    ++ticks;

    if (ticks >= TEMP_DELAY) {
        const int16_t temp_int = system_get_temp();
        system_send_temp(temp_int);
        system_display_temp(temp_int);

        ticks = 0;
    }
}
