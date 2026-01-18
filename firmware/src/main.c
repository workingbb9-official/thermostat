#include <firmware/sys_core.h>

int main(void) {
    sys_init();

	for (;;) {
        sys_run();
	}

	return 0;
}
