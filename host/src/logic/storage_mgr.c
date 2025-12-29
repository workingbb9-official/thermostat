#include "logic/storage_mgr.h"

#include <stdio.h>

#include "services/file_utils.h"
#include "services/port.h"

static int temp_fd = -1;

int storage_mgr_init(void) {
    temp_fd = port_open("host/data/temperature.txt");
    if (temp_fd < 0) {
        return 1;
    }

    return 0;
}

int storage_mgr_write_temp(float data) {
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%.2f", data);

    if (file_write_line(temp_fd, buffer, sizeof(buffer)) < 0) {
        return 1;
    }

    return 0;
}

int storage_mgr_close(void) {
    if (port_close(temp_fd) != 0) {
        return 1;
    }

    return 0;
}
