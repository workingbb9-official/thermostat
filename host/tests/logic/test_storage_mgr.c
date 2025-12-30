#include "logic/storage_mgr.h"

#include <stdio.h>

int main(void) {
    
    if (storage_mgr_init() != 0) {
        printf("Init failure\n");
        return 1;
    } else {
        printf("Init storage manager\n");
    }

    if (storage_mgr_write_temp(3.1456) != 0) {
        printf("Write failure\n");
        goto cleanup;
    } else {
        printf("Wrote 3.1456\n");
    }
    
    float buffer;
    int read_status = storage_mgr_read_temp(&buffer, 5);
    switch (read_status) {
        case -1:
            printf("Invalid params to read\n");
            goto cleanup;
        case -2:
            printf("System error with read\n");
            goto cleanup;
        case 0:
            printf("Read: %f\n", buffer);
            break;
        default:
            printf("Unexpected return from read\n");
            goto cleanup;
    }

    if (storage_mgr_close() != 0) {
        printf("Close failure\n");
        return 1;
    } else {
        printf("Close storage manager\n");
    }

    return 0;

    cleanup:
    if (storage_mgr_close() != 0) {
        printf("Close failure\n");
    } else {
        printf("Close storage manager\n");
    }

    return 1;
}
