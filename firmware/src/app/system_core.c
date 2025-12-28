#include "app/system_core.h"

#include <stdlib.h>

#include "logic/therm_mgr.h"
#include "logic/uart_mgr.h"

void system_init(void) {
    therm_mgr_init();
    uart_mgr_init();
}

void system_send_temp(void) {
    const float temp_c = therm_mgr_get_temp();

    char buff[16];
    dtostrf(temp_c, 4, 2, buff);
    uart_mgr_transmit_string(buff);
}
