#include "logic/port_mgr.h"

#include "services/port.h"

int port_mgr_init(const char *port_path) {
    int port = port_open(port_path);
    if (port < 0) {
        return -1;
    }

    int config_status = port_configure(port, 115200);
    if (config_status != 0) {
        return -1;
    }

    return port;
}
