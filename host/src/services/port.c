#include "port.h"

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

static speed_t numeric_to_baud(int rate) {
    switch (rate) {
        case 9600: return B9600;
        case 115200: return B115200;
        default: return B0;
    }
}

int port_configure(int port, int speed) {
    speed_t baud_rate = numeric_to_baud(speed);
    if (baud_rate == B0) {
        printf("Invalid baud rate\n");
        return 1;
    }

    struct termios tty;

    // Get current attributes
    if (tcgetattr(port, &tty) != 0) {
        printf("Error from tcgetattr\n");
        return 1;
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

    tty.c_cc[VMIN] = 1; // Fetch one char at a time

    // Set new attributes
    if (tcsetattr(port, 0, &tty) != 0) {
        printf("Error from tcsetattr\n");
        return 1;
    }

    return 0;
}

/* size_t port_read(int port) {
    char binary[256];
    int bytes = read(port, &binary, sizeof(binary));
    if (bytes < 0) {
        printf("Error from read\n");
        return 1;
    }
    
    size_t binary_size = sizeof(binary) / binary[0];
    char output[256];
    for (size_t i = 0; i < binary_size; ++i) {
        output[i] = (char)binary[i] */
