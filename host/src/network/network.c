#include <host/network.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int net_dev_init(struct net_device *dev, const struct net_driver *drv, 
                 const char *host, const char *path)
{
    if (!dev || !drv || ! host || !path)
        return -1;

    dev->drv = drv;
    dev->host = host;
    dev->path = path;
    
    return 0;
}




/* For the http driver */
static int http_fetch(const struct net_device *dev, 
                      char *buf, size_t buf_size);

const struct net_driver g_http_drv = {
    .fetch = http_fetch
};

static int http_fetch(const struct net_device *dev, 
                      char *buf, size_t buf_size)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return -1;

    struct hostent *he = gethostbyname(dev->host);
    if (!he) {
        close(sock);
        return -1;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    memcpy(&server.sin_addr, he->h_addr, he->h_length);

    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        close(sock);
        return -1;
    }

    char request[1024] = {0};
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", 
            dev->path, dev->host);
    write(sock, request, strlen(request));
    
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
    return 0;
}
