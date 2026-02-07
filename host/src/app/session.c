#include "session.h"

#include <stdio.h>
#include <time.h>

#include <host/common/tsys_errors.h>
#include <host/file_utils.h>

static int get_time(char *buf, size_t buf_size)
{
    const time_t time_abs = time(NULL);
    struct tm *const time = localtime(&time_abs);

    // Convert timestamp to string
    const int data_len = snprintf(
        buf,
        buf_size,
        "%d-%d-%d",
        time->tm_year,
        time->tm_mon,
        time->tm_mday);

    const int time_len = snprintf(
        buf + data_len,
        buf_size - data_len,
        "T%d:%d:%d",
        time->tm_hour,
        time->tm_min,
        time->tm_sec);

    return data_len;
}

enum tsys_err session_record_login(int session_fd)
{
    if (session_fd < 0) {
        printf("Record login: invalid fd\n");
        return TSYS_E_INVAL;
    }

    // Seek to end of file
    if (file_seek(session_fd, END) < 0) {
        printf("Record login: failed to seek\n");
        return TSYS_E_FILE;
    }

    // Buf must be at least 17 bytes
    char buf[32];
    const int time_start = get_time(buf, sizeof(buf)) + 1;
    printf("Login at %s\n", buf + time_start);

    // Append 'I' to signify login
    char line[32];
    const int len = snprintf(line, sizeof(line), "%c%s", 'I', buf);

    // Write timestamp to session file
    if (file_write_line(session_fd, line, len) < 0) {
        printf("Record login: failed to write line\n");
        return TSYS_E_FILE;
    }

    return TSYS_OK;
}

enum tsys_err session_record_logout(int session_fd)
{
    if (session_fd < 0) {
        printf("Record logout: invalid fd\n");
        return TSYS_E_INVAL;
    }

    // Seek to end of file
    if (file_seek(session_fd, END) < 0) {
        printf("Record logout: failed to seek\n");
        return TSYS_E_FILE;
    }

    // Buf must be at least 17 bytes
    char buf[32];
    const int time_start = get_time(buf, sizeof(buf)) + 1;
    printf("Logout at %s\n", buf + time_start);

    // Append 'O' to signify logout
    char line[32];
    const int len = snprintf(line, sizeof(line), "%c%s", 'O', buf);

    // Write timestamp to session file
    if (file_write_line(session_fd, line, len) < 0) {
        printf("Record logout: failed to write line\n");
        return TSYS_E_FILE;
    }

    return TSYS_OK;
}
