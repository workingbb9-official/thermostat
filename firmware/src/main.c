#include "app/system_core.h"

#define EXIT_SUCCESS 0

int main() {
    system_init();

	while (1) {
        system_send_temp();
	}

	return EXIT_SUCCESS;
}
