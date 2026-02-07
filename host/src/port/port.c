#include <host/port.h>

#include "port_hal.h"
#include <thermostat/protocol.h>

static int port_fd = -1;

static inline int write_byte(char byte)
{
    return port_hal_write(port_fd, &byte, 1);
}

enum port_err port_init(void)
{
    port_fd = port_hal_open("/dev/ttyACM0");
    if (port_fd < 0) {
        return PORT_E_OPEN;
    }

    int config_status = port_hal_configure(port_fd, 115200);
    if (config_status != 0) {
        return PORT_E_CONFIG;
    }

    return PORT_OK;
}

enum port_err port_read_byte(char *buf)
{
    if (!buf) {
        return PORT_E_INVAL;
    }

    if (port_hal_read(port_fd, buf, 1) < 0) {
        return PORT_E_READ;
    }

    return PORT_OK;
}

enum port_err port_send_packet(struct data_packet *pkt)
{
    if (!pkt) {
        return PORT_E_INVAL;
    }

    if (write_byte(pkt->start_byte) < 0) {
        return PORT_E_WRITE;
    }

    if (write_byte(pkt->type) < 0) {
        return PORT_E_WRITE;
    }

    if (write_byte(pkt->length) < 0) {
        return PORT_E_WRITE;
    }

    for (int i = 0; i < pkt->length; ++i) {
        if (write_byte(pkt->payload[i]) < 0) {
            return PORT_E_WRITE;
        }
    }

    if (write_byte(pkt->checksum) < 0) {
        return PORT_E_WRITE;
    }

    return PORT_OK;
}

enum port_err port_close(void)
{
    return port_hal_close(port_fd);
}
