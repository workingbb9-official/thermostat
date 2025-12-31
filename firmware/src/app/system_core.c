#include "app/system_core.h"

#include "logic/therm_mgr.h"
#include "logic/uart_mgr.h"

void system_init(void) {
    therm_mgr_init();
    uart_mgr_init();
}
