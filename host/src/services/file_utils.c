#include "services/file_utils.h"

#include <unistd.h>
#include <fcntl.h>

int file_open(const char *file_path) {
    return open(file_path, O_RDWR);
}

ssize_t file_read_line(int fd, char *buffer, size_t buffer_size, int line) {
    if (fd < 0 || 
        !buffer ||
        buffer_size == 0 ||
        line <= 0) {
        return -1;
    }
    
    int current_line = 1; 

    while (current_line < line) {
        char c;
        ssize_t bytes_read = read(fd, &c, 1);

        if (bytes_read == 0) {
            return 0;
        } else if (bytes_read < 0) {
            return -2;
        } 

        if (c == '\n') {
            ++current_line;
        }
    }

    int pos = 0;

    while (pos < buffer_size - 1) { 
        ssize_t bytes_read = read(fd, &buffer[pos], 1); 

        if (bytes_read == 0) {
            buffer[pos] = '\0';
            if (pos > 0) {
                return pos;
            }
            
            return 0; 
        } else if (bytes_read < 0) {
            buffer[pos] = '\0';
            return -2;
        }

        if (buffer[pos] == '\n') {
            buffer[pos] = '\0';
            return pos;
        }
        
        ++pos;
    }
    
    buffer[pos] = '\0';
    return -1;
}

ssize_t file_write_line(int fd, const char *text, size_t text_len) {
    if (fd < 0 || 
        !text) {
        return -1;
    }

    ssize_t bytes_wrote = write(fd, text, text_len);
    ssize_t new_line = write(fd, "\n", 1);

    if (bytes_wrote < 0 || new_line < 1) {
        return -2;
    }

    return bytes_wrote;
}

int file_seek(int fd, int position) {
    if (fd < 0) {
        return -1;
    }

    if (position == START) {
        lseek(fd, 0, SEEK_SET);
    } else if (position == END) {
        lseek(fd, 0, SEEK_END);
    } else {
        return -1;
    }

    return 0;
}

int file_close(int fd) {
    return close(fd);
}
