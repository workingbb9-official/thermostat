#include "port_hal.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

static speed_t numeric_to_baud(int rate);

int port_hal_open(const char *file_path) {
    if (!file_path)
        return PORT_E_INVAL;

    int port = open(file_path, O_RDWR | O_NOCTTY);
    if (port < 0)
        return PORT_E_OPEN;

    return port;
}

enum port_err port_hal_configure(int port, int speed) {
    speed_t baud_rate = numeric_to_baud(speed);
    if (baud_rate == B0 || port < 0)
        return PORT_E_INVAL;


    struct termios tty = {0};

    // Get current attributes
    if (tcgetattr(port, &tty) < 0)
        return PORT_E_CONFIG;


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
    if (tcsetattr(port, 0, &tty) < 0)
        return PORT_E_CONFIG;

    return PORT_OK;
}

ssize_t port_hal_read(int port, char *buf, size_t bytes) {
    if (port < 0 || !buf)
        return PORT_E_INVAL;


    ssize_t bytes_read = read(port, buf, bytes);
    if (bytes_read < 0)
        return PORT_E_READ;
    
    buf[bytes_read] = '\0';
    return bytes_read;
}

ssize_t port_hal_write(int port, const char *buf, size_t bytes) {
    if (port < 0 || !buf)
        return PORT_E_INVAL;

    ssize_t bytes_written = write(port, buf, bytes);
    if (bytes_written < 0)
        return PORT_E_READ;

    return bytes_written;
}

enum port_err port_hal_close(int port) {
    if (port < 0)
        return PORT_E_INVAL;
    
    if (close(port) < 0)
        return PORT_E_CLOSE;

    return PORT_OK;
}

static speed_t numeric_to_baud(int rate) {
    switch (rate) {
    case 9600: return B9600;
    case 115200: return B115200;
    default: return B0;
    }
}
