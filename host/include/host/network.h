#ifndef NETWORK_H
#define NETWORK_H

#include <sys/types.h>

// NET errors run from -29 to -20
enum net_err {
    NET_OK = 0,
    NET_E_INVAL = -29,
    NET_E_SYS
};

__attribute__((visibility("hidden"))) struct net_ops;
extern const struct net_ops http_ops;

struct net_device {
    const struct net_ops *ops;
    const char *host;
    const char *path;
};

/**
 * @brief Initialize network device
 *
 * @param dev - Device to init
 * @param ops - Net ops for device, determines fetching
 * @param host - Host URL (eg. youtube.com)
 * @param path - Path to website (eg. /feed/history)
 *
 * @retval NET_OK - Init was successful
 * @retval NET_E_INVAL - Invalid param
 */
enum net_err net_dev_init(
    struct net_device *dev,
    const struct net_ops *ops,
    const char *host,
    const char *path);

/**
 * @brief: Fetch website response
 *
 * This function will go to the host from the dev.
 * Then it will request to retrieve the path given by dev.
 * The given string is stored in buf, which is null-terminated.
 *
 * @param dev - Initialized net device
 * @param buf - Buffer to hold response
 * @param buf_size - Size of the buffer
 *
 * @return Bytes read into buffer, or NET error
 * @retval NET_E_INVAL - Invalid param
 * @retval NET_E_SYS - System error
 */
ssize_t net_dev_fetch(
    const struct net_device *dev,
    char *buf,
    size_t buf_size);

#endif // NETWORK_H
