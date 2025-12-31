#include "app/system_core.h"

int main(void) {
    system_init();
    system_analyze();

    for (;;) {
        system_run();
    }

    system_cleanup();
    return 0;
}
