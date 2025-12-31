#include "services/port.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

static speed_t numeric_to_baud(int rate);

int port_open(const char *file_path) {
    return open(file_path, O_RDWR | O_NOCTTY);
}

int port_configure(int port, int speed) {
    speed_t baud_rate = numeric_to_baud(speed);
    if (baud_rate == B0 || port < 0) {
        return -1;
    }

    struct termios tty;

    // Get current attributes
    if (tcgetattr(port, &tty) != 0) {
        return -2;
    }

    tty.c_cflag |= (CREAD | CLOCAL); // Set to read and ignore modem controls
    cfsetspeed(&tty, baud_rate); // Set baud rate

    // Clear and set data size
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    // No parity, 1 stop bit, no flow control
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    // Set up non-canonical mode (raw input)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~IXON;
    tty.c_iflag &= ~IXANY;
    tty.c_iflag &= ~(
        IGNBRK | BRKINT | PARMRK
        | ISTRIP | INLCR | IGNCR |ICRNL
    );
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] = 1; // Fetch one byte at a time

    // Set new attributes
    if (tcsetattr(port, 0, &tty) != 0) {
        return -2;
    }

    return 0;
}

int port_read(int fd, char *buffer, int bytes) {
    if (fd < 0 ||
        !buffer) {
        return -1;
    }

    if (read(fd, buffer, bytes) != bytes) {
        return -2;
    }

    return 0;
}

int port_close(int fd) {
    return close(fd);
}

static speed_t numeric_to_baud(int rate) {
    switch (rate) {
    case 9600: return B9600;
    case 115200: return B115200;
    default: return B0;
    }
}
