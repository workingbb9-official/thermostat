#include "services/file_utils.h"

#include <unistd.h>

ssize_t file_read_line(int fd, char *buffer, size_t buffer_size, int line) {
    if (fd < 0 || 
        buffer_size < 0 || 
        !buffer ||
        line <= 0) {
        return -2;
    }
    
    int pos = 0;
    int current_line = 1; 

    while (current_line < line) {
        char c;
        ssize_t bytes_read = read(fd, &c, 1);

        if (bytes_read == 0) {
            return 0;
        } else if (bytes_read < 0) {
            return -1;
        } 

        if (c == '\n') {
            ++current_line;
        }
    }

    while (pos < buffer_size - 1) { 
        ssize_t bytes_read;
        bytes_read = read(fd, &buffer[pos], 1); 

        if (bytes_read == 0) {
            if (pos > 0) {
                buffer[pos] = '\0';
                close(fd);
                return pos;
            }

            return 0; 
        } else if (bytes_read < 0) {
            return -1;
        }

        if (buffer[pos] == '\n') {
            buffer[pos] = '\0';
            return pos;
        }
        
        ++pos;
    }
    
    return -1;
}

ssize_t file_write_line(int fd, const char *text, size_t text_size) {
    if (fd < 0 || 
        text_size <= 0 || 
        !text) {
        return -2;
    }

    ssize_t bytes_wrote = write(fd, text, text_size - 1);
    ssize_t new_line = write(fd, "\n", 1);

    return bytes_wrote;
}
