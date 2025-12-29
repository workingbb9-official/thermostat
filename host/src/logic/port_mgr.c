#include "logic/port_mgr.h"

#include "services/port.h"

#define NULL ((void *)0)

static int port_fd = -1;

int port_mgr_init(void) {
    port_fd = port_open("/dev/ttyACM0");
    if (port_fd < 0) {
        return -1;
    }

    int config_status = port_configure(port_fd, 115200);
    if (config_status != 0) {
        return -1;
    }

    return port_fd;
}

int port_mgr_read_byte(char *buffer) {
    if (buffer == NULL) {
        return 1;
    }

    if (port_read(port_fd, buffer, 1) != 0) {
        return 1;
    }

    return 0;
}

int port_mgr_close(void) {
    return port_close(port_fd);
}
