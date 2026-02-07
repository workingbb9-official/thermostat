#include <firmware/sys_core.h>

int main(void)
{
    sys_init();

    while (1) {
        sys_run();
    }

    return 0;
}
