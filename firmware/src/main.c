#include "app/system_core.h"
#include "app/system_temperature.h"

int main(void) {
    system_init();

	for (;;) {
        system_send_temp();
	}

	return 0;
}
