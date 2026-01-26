#include <host/sys_core.h>
#include <host/common/tsys_errors.h>
#include <stdio.h>

int main(void) {
    if (sys_init() < TSYS_OK) {
        printf("System init error\n");
        return 1;
    }
    
    printf("System initialized\n\n");

    while (!sys_should_shutdown()) {
        sys_run();
    }

    if (sys_cleanup() < TSYS_OK) {
        printf("System cleanup error\n");
        return 1;
    }

    return 0;
}
