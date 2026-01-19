#include <host/network.h>
#include <stdio.h>

#define BUF_SIZE 1024

int main(void) {
    struct net_device http_dev = {0};
    net_dev_init(&http_dev, &g_http_drv, "httpbin.org", "/get");

    char buf[BUF_SIZE] = {0};
    if (http_dev.drv->fetch(&http_dev, buf, BUF_SIZE) < 0) {
        printf("Failed to fetch dev1\n");
        return 1;
    }
    
    printf("From dev1: %s\n", buf);

    struct net_device http_dev2 = {0};
    net_dev_init(&http_dev2, &g_http_drv, "httpbin.org", "/post");

    if (http_dev2.drv->fetch(&http_dev2, buf, BUF_SIZE) < 0) {
        printf("Failed to fetch dev2\n");
        return 1;
    }

    printf("From dev2: %s\n", buf);

    return 0;
}
