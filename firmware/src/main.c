#include "system_core.h"

int main(void) {
    system_init();

	for (;;) {
        system_run();
	}

	return 0;
}
