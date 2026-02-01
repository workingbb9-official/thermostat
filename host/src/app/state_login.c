#include "state_login.h"

#include <stdio.h>
#include <time.h>

#include <host/common/tsys_errors.h>
#include <host/file_utils.h>

enum tsys_err login_record_login(int login_fd) {
    const time_t time_abs = time(NULL);
    struct tm * const time = localtime(&time_abs);
    printf("Login at %d:%d:%d\n", time->tm_hour, time->tm_min, time->tm_sec);
    
    // Convert timestamp to string
    char buf[32];
    const int len = snprintf(
            buf, sizeof(buf), "%d-%d-%dT%d:%d:%d",
            time->tm_year, time->tm_mon, time->tm_mday, 
            time->tm_hour, time->tm_min, time->tm_sec);

    // Seek to end to setup writing
    if (file_seek(login_fd, END) < 0) {
        printf("Record login: failed to seek\n");
        return TSYS_E_FILE;
    }

    // Write timestamp to login file
    if (file_write_line(login_fd, buf, len) < 0) {
        printf("Record login: failed to write line\n");
        return TSYS_E_FILE;
    }

    return TSYS_OK;
}
