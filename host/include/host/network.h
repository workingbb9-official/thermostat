#ifndef NETWORK_H
#define NETWORK_H

#include <sys/types.h>

// NET errors run from -29 to -20
typedef enum net_err {
    NET_OK = 0,
    NET_E_SYS = -29,
    NET_E_INVAL
} net_err_t;

__attribute__((visibility("hidden"))) struct net_ops;
extern const struct net_ops http_ops;

struct net_device {
    const struct net_ops *ops;
    const char *host;
    const char *path;
};

/*
 * Initialize network device
 *
 * Params:
 *      dev: Ptr to device to init
 *      ops: Ptr to net ops, determines fetching
 *      host: Host URL (eg. youtube.com)
 *      path: Path to website (eg. /feed/history)
 * 
 * Return:
 *      NET_OK: Init was successful
 *      NET_E_INVAL: Invalid param
 *
 */
net_err_t net_dev_init(
    struct net_device *dev,
    const struct net_ops *ops,
    const char *host,
    const char *path
);

/*
 * Fetch website response
 *
 * Params:
 *      dev: Ptr to initialized net device
 *      buf: Buffer to hold response
 *      buf_size: Size of the buffer
 *
 * Return:
 *      >0: Bytes read into buffer
 *      NET_E_INVAL: Invalid param
 *      NET_E_SYS: System error
 *
 * Notes:
 *      This function will go to the host from the dev.
 *      Then it will request to retrieve the path given by dev.
 *      The given string is stored in buf, which is null-terminated.
 */
ssize_t net_dev_fetch(
    const struct net_device *dev,
    char *buf,
    size_t buf_size
);

#endif // NETWORK_H
