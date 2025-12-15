#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "port.h"

int main(void) {
    int port = open("/dev/ttyACM0", O_RDONLY);
    if (port < 0) {
        printf("Failed to open port\n");
        return 1;
    }

    if (port_configure(port, 115200) != 0) {
        printf("Failed to configure port\n");
        return 1;
    }

    char c;
    while (read(port, &c, 1) == 1) {
        putchar(c);
        fflush(stdout);
    }

    close(port);
    return 0;
}
