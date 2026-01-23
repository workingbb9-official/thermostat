#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

#define NET_OK        0
#define NET_E_INVAL  -1
#define NET_E_IO     -2

__attribute__((visibility("hidden"))) struct net_ops;
extern const struct net_ops http_ops;

struct net_device {
    const struct net_ops *ops;
    const char *host;
    const char *path;
};

/**
 * @brief Initialize net device
 * @param *dev      Ptr to net device
 * @param *ops      Ptr to chosen net ops
 * @param *host     String for the host (eg. google.com)
 * @param *path     String for the path (eg. /path/to/dest)
 * @return int      NET error code
 */
int net_dev_init(struct net_device *dev, 
                 const struct net_ops *ops, 
                 const char *host, 
                 const char *path);

/**
 * @brief Fetch raw website data from internet
 * @param *dev      Ptr to initialized net device
 * @param *buf      Buffer to hold output
 * @param buf_size  Size of the buffer
 * @return int      NET error code
 */
int net_dev_fetch(const struct net_device *dev,
                  char *buf,
                  size_t buf_size);

#endif // NETWORK_H
