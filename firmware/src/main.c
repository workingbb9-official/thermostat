#include <util/delay.h>

#include "app/system_core.h"

#define EXIT_SUCCESS 0

int main() {
    system_init();

	while (1) {
        system_send_temp();

        _delay_ms(3000);
	}

	return EXIT_SUCCESS;
}
