#include <host/file_mgr.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include <host/common/file_errors.h>

static int temp_fd = -1;

enum file_err file_mgr_init(void) {
    temp_fd = file_open("host/data/temperature.txt");
    if (temp_fd < 0) {
        return FILE_E_OPEN;
    }

    return FILE_OK;
}

enum file_err file_mgr_reset_temp(void) {
    if (file_seek(temp_fd, START) < 0) {
        return FILE_E_SEEK;
    }

    return FILE_OK;
}

enum file_err file_mgr_write_temp(float data) {
    // Convert to string
    char buf[16];
    const int len = snprintf(buf, sizeof(buf), "%.2f", data);
    buf[len] = '\0';

    if (len < 0)
        return FILE_E_SEEK;

    if (file_seek(temp_fd, END) < 0)
        return FILE_E_SEEK;

    if (file_write_line(temp_fd, buf, len) <= 0)
        return FILE_E_WRITE;

    return FILE_OK;
}

enum file_err file_mgr_read_temp(float *val_out) {
    if (!val_out)
        return FILE_E_INVAL;

    // Temporary buffer
    char text[32] = {0};
    if (file_read_line(temp_fd, text, sizeof(text) - 1, 1) <= 0)
        return FILE_E_READ;

    // Convert to float and store
    *val_out = strtof(text, NULL);

    return FILE_OK;
}

enum file_err file_mgr_close(void) {
    if (file_close(temp_fd) < 0)
        return FILE_E_CLOSE;

    return FILE_E_OPEN;
}
