#include "file_utils.h"

#include <unistd.h>
#include <fcntl.h>

#include <host/common/file_errors.h>

int file_open(const char *file_path) {
    int file = open(file_path, O_RDWR);
    if (file < 0)
        return FILE_E_OPEN;

    return file;
}

ssize_t file_read_line(int file, char *buf_out, size_t bytes, size_t line) {
    if (file < 0 || !buf_out)
        return FILE_E_INVAL;
    
    // Move through lines until we reach current
    int current_line = 1;
    while (current_line < line) {
        char c;
        ssize_t bytes_read = read(file, &c, 1);

        if (bytes_read == 0)
            return 0;
        else if (bytes_read < 0)
            return FILE_E_READ;

        if (c == '\n')
            ++current_line;
    }

    int pos = 0;

    while (pos < bytes - 1) {
        ssize_t bytes_read = read(file, &buf_out[pos], 1);
        
        // Check if reached EOF
        if (bytes_read == 0) {
            buf_out[pos] = '\0';
            return pos;
        } else if (bytes_read < 0) {
            buf_out[pos] = '\0';
            return FILE_E_READ;
        }
        
        // Check for end of line
        if (buf_out[pos] == '\n') {
            buf_out[pos] = '\0';
            return pos;
        }

        ++pos;
    }

    buf_out[pos] = '\0';
    return FILE_E_READ;
}

ssize_t file_write_line(int file, const char *buf, size_t bytes) {
    if (file < 0 || !buf)
        return FILE_E_INVAL;

    ssize_t bytes_written = write(file, buf, bytes);
    ssize_t new_line = write(file, "\n", 1);

    if (bytes_written < 0 || new_line < 1)
        return FILE_E_WRITE;

    return bytes_written;
}

enum file_err file_seek(int file, int pos) {
    if (file < 0)
        return FILE_E_INVAL;

    if (pos != START && pos != END)
        return FILE_E_INVAL;

    if (pos == START)
        if (lseek(file, 0, SEEK_SET) < 0)
            return FILE_E_SEEK;
    else
        if (lseek(file, 0, SEEK_END) < 0)
            return FILE_E_SEEK;

    return FILE_OK;
}

enum file_err file_close(int file) {
    int close_status = close(file);
    if (close_status < 0)
        return FILE_E_CLOSE;

    return FILE_OK;
}
