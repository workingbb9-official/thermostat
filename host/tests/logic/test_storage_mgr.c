#include "logic/storage_mgr.h"

#include <stdio.h>


int main(void) {
   
    if (storage_mgr_init() != 0) {
        printf("Failed to init\n");
        return 1;
    } else {
        printf("Succeeded init\n");
    }
    if (storage_mgr_write_temp(3.1415f) != 0) {
        printf("Failed to write\n");
        return 1;
    } else {
        printf("Succeeded write\n");
    }

    if (storage_mgr_close() != 0) {
        printf("Failed to close\n");
        return 1;
    } else {
        printf("Succeeded close\n");
    }

    return 0;
}
