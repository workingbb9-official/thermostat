#include "system_core.h"
#include <stdio.h>

int main(void) {
    if (system_init() != TSYS_OK) {
        printf("System init error\n");
        return 1;
    }

    if (system_analyze() != TSYS_OK) {
        printf("System analyze error\n");
        return 1;
    }

    while (!system_should_shutdown()) {
        system_run();
    }

    if (system_cleanup() != TSYS_OK) {
        printf("System cleanup error\n");
        return 1;
    }

    return 0;
}
