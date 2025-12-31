#include "app/system_core.h"
#include "app/system_temperature.h"

#define EXIT_SUCCESS 0

int main() {
    system_init();

	while (1) {
        system_send_temp();
	}

	return EXIT_SUCCESS;
}
