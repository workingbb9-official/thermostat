#include "session.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <host/common/tsys_errors.h>
#include <host/file_utils.h>
#include <host/port.h>
#include <thermostat/protocol.h>

static int get_timestamp(char *buf, size_t buf_size)
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

static void construct_auth_packet(
    struct data_packet *pkt,
    const char *user)
{
    if (!pkt) {
        return;
    }

    pkt->start_byte = START_BYTE;
    pkt->type = AUTH;

    if (user) {
        int i = 0;
        while (*user != '\0') {
            pkt->payload[i] = *user++;
            ++i;
        }
        pkt->payload[i] = '\0';
        pkt->length = i + 1;
    } else {
        pkt->length = 1;
        pkt->payload[0] = PAYLOAD_NONE;
    }

    pkt->checksum = 1;
}

static void find_pwd(char *line, char **buf)
{
    if (!line || !buf) {
        return;
    }

    char *temp = line;
    while (*temp != '-') {
        ++temp;
    }

    *buf = temp + 1;
}

enum tsys_err session_record_login(int session_fd)
{
    if (session_fd < 0) {
        printf("Record login: invalid fd\n");
        return TSYS_E_INVAL;
    }

    // Seek to end of file
    if (file_seek(session_fd, FILE_UTILS_END) < 0) {
        printf("Record login: failed to seek\n");
        return TSYS_E_FILE;
    }

    // Buf must be at least 17 bytes
    char buf[32];
    const int time_start = get_timestamp(buf, sizeof(buf)) + 1;
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
    if (file_seek(session_fd, FILE_UTILS_END) < 0) {
        printf("Record logout: failed to seek\n");
        return TSYS_E_FILE;
    }

    // Buf must be at least 17 bytes
    char buf[32];
    const int time_start = get_timestamp(buf, sizeof(buf)) + 1;
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

enum tsys_err session_validate_pwd(
    int pwd_fd,
    const char *pwd,
    char *user,
    int len)
{
    if (pwd_fd < 0 || !pwd || !user) {
        return TSYS_E_INVAL;
    }

    int ret;

    ret = file_seek(pwd_fd, FILE_UTILS_START);
    if (ret < 0) {
        return TSYS_E_FILE;
    }

    char line[64];
    while (1) {
        ret = file_read_line(pwd_fd, line, sizeof(line));
        if (ret < 0) {
            return TSYS_E_FILE;
        }

        if (ret == 0) {
            break;
        }

        char *pwd_valid;
        find_pwd(line, &pwd_valid);

        if (strcmp(pwd, pwd_valid) == 0) {
            int l = pwd_valid - line;
            if (l > len) {
                memcpy(user, line, len);
                user[len] = '\0';
            } else {
                memcpy(user, line, l - 1);
                user[l - 1] = '\0';
            }

            printf("Username: %s\n", user);
            return TSYS_OK;
        }
    }

    *user = '\0';

    return TSYS_OK;
}

enum tsys_err session_send_valid_pwd(const char *user)
{
    if (!user) {
        return TSYS_E_INVAL;
    }

    int ret;

    struct data_packet pkt = {0};
    construct_auth_packet(&pkt, user);

    usleep(250000);
    ret = port_send_packet(&pkt);
    if (ret < 0) {
        return TSYS_E_PORT;
    }

    return TSYS_OK;
}

enum tsys_err session_send_invalid_pwd(void)
{
    int ret;

    struct data_packet pkt = {0};
    construct_auth_packet(&pkt, NULL);

    usleep(250000);
    ret = port_send_packet(&pkt);
    if (ret < 0) {
        return TSYS_E_PORT;
    }

    return TSYS_OK;
}
