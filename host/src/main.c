#include "app/system_core.h"

int main(void) {
    system_init();
    system_analyze();

    while (!system_should_shutdown()) {
        system_run();
    }

    system_cleanup();
    return 0;
}
