#include <host/storage.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"

static int temp_fd = -1;

int storage_mgr_init(void) {
    temp_fd = file_open("host/data/temperature.txt");
    if (temp_fd < 0) {
        return -1;
    }

    return 0;
}

int storage_mgr_write_temp(float data) {
    char buffer[16];
    const int len = snprintf(buffer, sizeof(buffer) - 1, "%.2f", data);
    buffer[len] = '\0';

    
    if (file_seek(temp_fd, END) != 0) {
        return -1;
    }

    if (file_write_line(temp_fd, buffer, len) < 0) {
        return -1;
    }

    return 0;
}

int storage_mgr_read_temp(float *buffer, int line) {
    if (!buffer || line <= 0) {
        return -1;
    }
    
    if (file_seek(temp_fd, START) != 0) {
        return -2;
    }

    char text[32];
    if (file_read_line(temp_fd, text, sizeof(text) - 1, line) <= 0) {
        return -2;
    }
    
    *buffer = strtof(text, NULL);

    return 0;
}

int storage_mgr_close(void) {
    return file_close(temp_fd);
}
