#include <host/network.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/* Private definition */
__attribute__((visibility("hidden"))) struct net_ops {
    ssize_t (*fetch)(
        const struct net_device *dev, 
        char *buf, 
        size_t buf_size);
};

/* Public API */
enum net_err net_dev_init(
    struct net_device *dev, 
    const struct net_ops *ops, 
    const char *host, 
    const char *path)
{
    if (!dev || !ops || !host || !path)
        return NET_E_INVAL;

    dev->ops = ops;
    dev->host = host;
    dev->path = path;
    
    return NET_OK;
}

ssize_t net_dev_fetch(
    const struct net_device *dev,
    char *buf,
    size_t buf_size)
{
    if (!dev || !buf || !dev->ops || !dev->ops->fetch)
        return NET_E_INVAL;

    return dev->ops->fetch(dev, buf, buf_size);
}

/* HTTP ops */
static ssize_t http_fetch(const struct net_device *dev, 
                      char *buf, 
                      size_t buf_size)
{
    if (!dev || !buf)
        return NET_E_INVAL;

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return NET_E_SYS;

    // Create host
    struct hostent *he = gethostbyname(dev->host);
    if (!he) {
        close(sock);
        return NET_E_SYS;
    }

    // Create address
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    memcpy(&server.sin_addr, he->h_addr, he->h_length);

    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        close(sock);
        return NET_E_SYS;
    }

    char request[1024] = {0};
    snprintf(request, 
            sizeof(request),
            "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
            dev->path, 
            dev->host);

    // Send request
    if (write(sock, request, strlen(request)) < 0) {
        close(sock);
        return NET_E_SYS;
    }

    // Read response
    size_t total = 0;
    ssize_t n;
    while (total < buf_size - 1) {
        n = read(sock, buf + total, buf_size - total - 1);
        if (n <= 0)
            break;
        total += n;
    }
    buf[total] = '\0';

    close(sock);
    return total;
}

const struct net_ops http_ops = {
    .fetch  = http_fetch
};
