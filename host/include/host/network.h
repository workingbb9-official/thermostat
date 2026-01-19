#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

struct net_device {
    const struct net_driver *drv;
    const char *host;
    const char *path;
};

int net_dev_init(struct net_device *dev, const struct net_driver *drv, 
             const char *host, const char *path);

struct net_driver {
    int (*fetch)(const struct net_device *dev, char *buf, size_t buf_size);
};

extern const struct net_driver g_http_drv;

#endif // NETWORK_H
