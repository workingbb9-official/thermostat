#include "services/file_utils.h"

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>

int main(void) {
    int fd = open("host/test/data/data.txt", O_RDWR | O_APPEND);

    char buffer[1024];
    ssize_t bytes_read = file_read_line(fd, buffer, sizeof(buffer), 2);
    if (bytes_read <= 0) {
        printf("Error with read\n");
    }
    printf("Read: %s\n", buffer);

    const char text[] = "Spongeboy get back on the grill";
    ssize_t bytes_wrote = file_write_line(fd, text, sizeof(text));
    if (bytes_wrote == sizeof(text)) {
        printf("Write successful\n");
    }

    return 0;
}
