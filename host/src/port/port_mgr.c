#include <host/port.h>

#include "port_hal.h"
#include <thermostat/protocol.h>

static int port_fd = -1;

static int port_mgr_write_byte(char byte);

int port_mgr_init(void) {
    port_fd = port_open("/dev/ttyACM0");
    if (port_fd < 0) {
        return -1;
    }

    int config_status = port_configure(port_fd, 115200);
    if (config_status != 0) {
        return -1;
    }

    return 0;
}

int port_mgr_read_byte(char *buffer) {
    if (!buffer) {
        return -1;
    }

    if (port_read(port_fd, buffer, 1) != 0) {
        return -2;
    }

    return 0;
}

int port_mgr_send_packet(struct data_packet *packet) {
    if (port_mgr_write_byte(packet->start_byte) != 0) {
        return -2;
    }
    if (port_mgr_write_byte(packet->type) != 0) {
        return -2;
    }
    if (port_mgr_write_byte(packet->length) != 0) {
        return -2;
    }

    for (uint8_t i = 0; i < packet->length; ++i) {
        if (port_mgr_write_byte(packet->payload[i]) != 0) {
            return -2;
        }
    }
    
    if (port_mgr_write_byte(packet->checksum) != 0) {
        return -2;
    }

    return 0;
}

int port_mgr_close(void) {
    return port_close(port_fd);
}

static int port_mgr_write_byte(char byte) {
    return port_write(port_fd, &byte, 1);
}


